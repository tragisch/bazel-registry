#!/usr/bin/env python3

"""Create or update a module entry in this Bazel registry.

This tool is intentionally tailored to this repository layout:

    modules/<name>/metadata.json
    modules/<name>/<version>/MODULE.bazel
    modules/<name>/<version>/source.json
    modules/<name>/<version>/overlay/BUILD.bazel

It supports two modes:

* interactive prompts
* JSON input via --input

The generated source.json includes SRI integrity for the source archive and all
overlay / patch files.
"""

from __future__ import annotations

import argparse
import base64
import dataclasses
import functools
import hashlib
import json
import pathlib
import re
import shutil
import urllib.request
from typing import Any


def _download(url: str) -> bytes:
    request = urllib.request.Request(url, headers={"User-Agent": "bazel-registry-add-module/1.0"})
    with urllib.request.urlopen(request) as response:
        return response.read()


def _integrity(data: bytes, algorithm: str = "sha256") -> str:
    if algorithm not in {"sha224", "sha256", "sha384", "sha512"}:
        raise ValueError(f"Unsupported integrity algorithm: {algorithm}")
    digest = getattr(hashlib, algorithm)(data).digest()
    return f"{algorithm}-" + base64.b64encode(digest).decode("ascii")


def _read_text(path: pathlib.Path) -> str:
    return path.read_text(encoding="utf-8")


def _write_text(path: pathlib.Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")


def _write_json(path: pathlib.Path, payload: dict[str, Any], *, sort_keys: bool = False) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8") as handle:
        json.dump(payload, handle, indent=4, sort_keys=sort_keys)
        handle.write("\n")


@functools.total_ordering
class Version:
    """Semver-ish ordering compatible with the common BCR version scheme."""

    @functools.total_ordering
    class Identifier:
        def __init__(self, raw: str):
            if not raw:
                raise ValueError("Version identifier must not be empty")
            self.value: int | str = int(raw) if raw.isnumeric() else raw

        def __eq__(self, other: object) -> bool:
            if not isinstance(other, Version.Identifier):
                return NotImplemented
            return type(self.value) == type(other.value) and self.value == other.value

        def __lt__(self, other: object) -> bool:
            if not isinstance(other, Version.Identifier):
                return NotImplemented
            if type(self.value) != type(other.value):
                return isinstance(self.value, int)
            return self.value < other.value

    def __init__(self, version: str):
        self.parts = [Version.Identifier(part) for part in version.split(".")]

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Version):
            return NotImplemented
        return self.parts == other.parts

    def __lt__(self, other: object) -> bool:
        if not isinstance(other, Version):
            return NotImplemented
        return self.parts < other.parts


@dataclasses.dataclass
class ModuleSpec:
    name: str
    version: str
    build_file: str
    url: str | None = None
    remote: str | None = None
    commit: str | None = None
    tag: str | None = None
    module_file: str | None = None
    strip_prefix: str | None = None
    path_strip: int | None = None
    overlay_files: list[str] = dataclasses.field(default_factory=list)
    license: str | None = None
    description: str | None = None
    homepage: str | None = None
    source_repository: str | None = None
    compatibility_level: int | None = None
    system: list[str] = dataclasses.field(default_factory=list)
    deps: list[str] = dataclasses.field(default_factory=list)
    patches: list[str] = dataclasses.field(default_factory=list)
    patch_strip: int = 1
    maintainers: list[dict[str, str]] = dataclasses.field(default_factory=list)
    preserve_extra_metadata: bool = True

    @classmethod
    def from_dict(cls, data: dict[str, Any]) -> "ModuleSpec":
        return cls(
            name=data["name"],
            version=data["version"],
            build_file=data["build_file"],
            url=data.get("url"),
            remote=data.get("remote"),
            commit=data.get("commit"),
            tag=data.get("tag"),
            module_file=data.get("module_file"),
            strip_prefix=data.get("strip_prefix"),
            path_strip=data.get("path_strip"),
            overlay_files=list(data.get("overlay_files", [])),
            license=data.get("license"),
            description=data.get("description"),
            homepage=data.get("homepage"),
            source_repository=data.get("source_repository"),
            compatibility_level=data.get("compatibility_level"),
            system=list(data.get("system", [])),
            deps=list(data.get("deps", [])),
            patches=list(data.get("patches", [])),
            patch_strip=int(data.get("patch_strip", 1)),
            maintainers=list(data.get("maintainers", [])),
            preserve_extra_metadata=bool(data.get("preserve_extra_metadata", True)),
        )


def _ensure_non_empty(value: str, field_name: str) -> str:
    stripped = value.strip()
    if not stripped:
        raise ValueError(f"{field_name} must not be empty")
    return stripped


def _prompt(prompt: str, default: str | None = None) -> str:
    suffix = f" [{default}]" if default is not None else ""
    raw = input(f"{prompt}{suffix}: ").strip()
    if raw:
        return raw
    return default or ""


def _prompt_bool(prompt: str, default: bool = False) -> bool:
    rendered_default = "Y/n" if default else "y/N"
    while True:
        raw = input(f"{prompt} [{rendered_default}]: ").strip().lower()
        if not raw:
            return default
        if raw in {"y", "yes"}:
            return True
        if raw in {"n", "no"}:
            return False
        print("Bitte mit y oder n antworten.")


def _split_csv(raw: str) -> list[str]:
    return [item.strip() for item in raw.split(",") if item.strip()]


def _parse_github_remote(value: str) -> tuple[str, str] | None:
    prefix = "https://github.com/"
    remote = value
    if remote.startswith(prefix):
        path = remote[len(prefix):]
    elif remote.startswith("git@github.com:"):
        path = remote[len("git@github.com:"):]
    else:
        return None

    path = path.removesuffix(".git").strip("/")
    parts = path.split("/")
    if len(parts) < 2 or not parts[0] or not parts[1]:
        return None
    return parts[0], parts[1]


def _derive_source_repository(url: str | None = None, remote: str | None = None) -> str | None:
    candidate = remote or url
    if not candidate:
        return None
    parsed = _parse_github_remote(candidate)
    if not parsed:
        return None
    owner, repo = parsed
    return f"github:{owner}/{repo}"


def _derive_archive_url(spec: ModuleSpec) -> str:
    if spec.url:
        return spec.url

    if not spec.remote:
        raise ValueError("Either url or remote must be provided")

    parsed = _parse_github_remote(spec.remote)
    if not parsed:
        raise ValueError(f"Unsupported remote for archive derivation: {spec.remote}")
    owner, repo = parsed

    if bool(spec.commit) == bool(spec.tag):
        raise ValueError("Git sources require exactly one of commit or tag")

    if spec.commit:
        return f"https://github.com/{owner}/{repo}/archive/{spec.commit}.tar.gz"
    return f"https://github.com/{owner}/{repo}/archive/refs/tags/{spec.tag}.tar.gz"


def _derive_strip_prefix(spec: ModuleSpec) -> str | None:
    if spec.strip_prefix:
        return spec.strip_prefix

    parsed = _parse_github_remote(spec.remote or spec.url or "")
    if not parsed:
        return None
    _, repo = parsed

    if spec.commit:
        return f"{repo}-{spec.commit}"
    if spec.tag:
        return f"{repo}-{spec.tag.removeprefix('v')}"
    return None


@dataclasses.dataclass(frozen=True)
class ResolvedSource:
    url: str
    strip_prefix: str | None
    source_repository: str | None


def _resolve_source(spec: ModuleSpec) -> ResolvedSource:
    return ResolvedSource(
        url=_derive_archive_url(spec),
        strip_prefix=_derive_strip_prefix(spec),
        source_repository=spec.source_repository or _derive_source_repository(spec.url, spec.remote),
    )


def spec_from_user_input() -> ModuleSpec:
    name = _ensure_non_empty(_prompt("Modulname"), "name")
    version = _ensure_non_empty(_prompt("Version"), "version")
    use_git_source = _prompt_bool("Git-Repository statt Archiv-URL verwenden?", False)
    url: str | None = None
    remote: str | None = None
    commit: str | None = None
    tag: str | None = None
    if use_git_source:
        remote = _ensure_non_empty(_prompt("Git-Remote"), "remote")
        revision = _ensure_non_empty(_prompt("Commit oder Tag"), "revision")
        if re.fullmatch(r"[0-9a-f]{7,40}", revision):
            commit = revision
        else:
            tag = revision
    else:
        url = _ensure_non_empty(_prompt("URL des Source-Archivs"), "url")
    build_file = _ensure_non_empty(_prompt("Pfad zur BUILD.bazel-Vorlage"), "build_file")
    module_file = _prompt("Pfad zur MODULE.bazel-Vorlage (optional)") or None
    strip_prefix = _prompt("strip_prefix (optional)") or None
    path_strip_raw = _prompt("path-strip (optional)")
    overlay_files = _split_csv(_prompt("Zusätzliche Overlay-Dateien (optional, kommasepariert)") or "")
    license_name = _prompt("Lizenz (optional)") or None
    description = _prompt("Beschreibung (optional)") or None
    homepage = _prompt("Homepage (optional)") or None
    source_repository = _prompt("Repository, z.B. github:owner/repo (optional)") or _derive_source_repository(url, remote)
    compatibility_raw = _prompt("compatibility_level für generierte MODULE.bazel (optional)")
    deps = []
    if not module_file:
        deps = _split_csv(_prompt("Dependencies als name@version (optional, kommasepariert)"))
    system = _split_csv(_prompt("Systeme (optional, z.B. macos,linux)") or "")
    patches = _split_csv(_prompt("Patch-Dateien (optional, kommasepariert)") or "")
    patch_strip = int(_prompt("patch_strip", "1") or "1")

    maintainers: list[dict[str, str]] = []
    while _prompt_bool("Maintainer hinzufügen?", False):
        maintainer = {"name": _ensure_non_empty(_prompt("  Name"), "maintainer.name")}
        email = _prompt("  E-Mail (optional)")
        github = _prompt("  GitHub-Username (optional)")
        if email:
            maintainer["email"] = email
        if github:
            maintainer["github"] = github
        maintainers.append(maintainer)

    return ModuleSpec(
        name=name,
        version=version,
        build_file=build_file,
        url=url,
        remote=remote,
        commit=commit,
        tag=tag,
        module_file=module_file,
        strip_prefix=strip_prefix,
        path_strip=int(path_strip_raw) if path_strip_raw else None,
        overlay_files=overlay_files,
        license=license_name,
        description=description,
        homepage=homepage,
        source_repository=source_repository,
        compatibility_level=int(compatibility_raw) if compatibility_raw else None,
        system=system,
        deps=deps,
        patches=patches,
        patch_strip=patch_strip,
        maintainers=maintainers,
    )


def _module_content(spec: ModuleSpec) -> str:
    if spec.module_file:
        return _read_text(pathlib.Path(spec.module_file))

    header = [
        '"""',
        f"{spec.name} module",
        '"""',
        "module(",
        f'    name = "{spec.name}",',
        f'    version = "{spec.version}",',
    ]
    if spec.compatibility_level is not None:
        header.append(f"    compatibility_level = {spec.compatibility_level},")
    header.extend([")", ""])

    dep_lines = []
    for dep in spec.deps:
        if "@" not in dep:
            raise ValueError(f"Dependency must look like name@version, got: {dep}")
        dep_name, dep_version = dep.split("@", 1)
        dep_lines.append(f'bazel_dep(name = "{dep_name}", version = "{dep_version}")')

    return "\n".join(header + dep_lines).rstrip() + "\n"


def _metadata_payload(existing: dict[str, Any] | None, spec: ModuleSpec) -> dict[str, Any]:
    metadata: dict[str, Any] = {}
    if existing and spec.preserve_extra_metadata:
        metadata.update(existing)

    if spec.homepage:
        metadata["homepage"] = spec.homepage

    if spec.description:
        metadata["description"] = spec.description
    elif existing and "description" in existing:
        metadata["description"] = existing["description"]

    if spec.maintainers:
        metadata["maintainers"] = spec.maintainers
    elif existing and "maintainers" in existing:
        metadata["maintainers"] = existing["maintainers"]

    resolved_source = _resolve_source(spec)
    if resolved_source.source_repository:
        metadata["repository"] = [resolved_source.source_repository]
    elif existing and "repository" in existing:
        metadata["repository"] = existing["repository"]

    versions = set(existing.get("versions", []) if existing else [])
    versions.add(spec.version)
    metadata["versions"] = sorted(versions, key=Version)

    if "yanked_versions" in metadata or spec.maintainers or spec.source_repository:
        metadata.setdefault("yanked_versions", existing.get("yanked_versions", {}) if existing else {})

    return metadata


def _source_payload(
    spec: ModuleSpec,
    overlay_dir: pathlib.Path,
    patches_dir: pathlib.Path | None,
    source_integrity: str,
) -> dict[str, Any]:
    resolved_source = _resolve_source(spec)
    payload: dict[str, Any] = {
        "name": spec.name,
        "url": resolved_source.url,
        "integrity": source_integrity,
    }
    if resolved_source.strip_prefix:
        payload["strip_prefix"] = resolved_source.strip_prefix
    if spec.path_strip is not None:
        payload["path-strip"] = spec.path_strip
    if spec.license:
        payload["license"] = spec.license
    if spec.system:
        payload["system"] = spec.system

    overlay_files = sorted(
        path.relative_to(overlay_dir).as_posix()
        for path in overlay_dir.rglob("*")
        if path.is_file() and path.name != "MODULE.bazel.lock"
    )
    if overlay_files:
        payload["overlay"] = {
            relative_path: _integrity((overlay_dir / relative_path).read_bytes())
            for relative_path in overlay_files
        }

    if patches_dir and patches_dir.exists():
        patch_files = sorted(path.relative_to(patches_dir).as_posix() for path in patches_dir.rglob("*") if path.is_file())
        if patch_files:
            payload["patches"] = {
                relative_path: _integrity((patches_dir / relative_path).read_bytes())
                for relative_path in patch_files
            }
            payload["patch_strip"] = spec.patch_strip

    return payload


def create_or_update_module(spec: ModuleSpec, registry_root: pathlib.Path) -> pathlib.Path:
    build_source = pathlib.Path(spec.build_file)
    if not build_source.is_file():
        raise FileNotFoundError(f"BUILD file not found: {build_source}")
    module_source = pathlib.Path(spec.module_file) if spec.module_file else None
    if module_source and not module_source.is_file():
        raise FileNotFoundError(f"MODULE file not found: {module_source}")

    for overlay_path in spec.overlay_files:
        path = pathlib.Path(overlay_path)
        if not path.is_file():
            raise FileNotFoundError(f"Overlay file not found: {path}")

    for patch_path in spec.patches:
        path = pathlib.Path(patch_path)
        if not path.is_file():
            raise FileNotFoundError(f"Patch file not found: {path}")

    module_root = registry_root / "modules" / spec.name
    version_root = module_root / spec.version
    overlay_dir = version_root / "overlay"
    patches_dir = version_root / "patches" if spec.patches else None
    metadata_path = module_root / "metadata.json"

    existing_metadata = json.loads(_read_text(metadata_path)) if metadata_path.exists() else None

    overlay_dir.mkdir(parents=True, exist_ok=True)
    if patches_dir:
        patches_dir.mkdir(parents=True, exist_ok=True)

    shutil.copy2(build_source, overlay_dir / "BUILD.bazel")
    for overlay_path in spec.overlay_files:
        overlay_source = pathlib.Path(overlay_path)
        shutil.copy2(overlay_source, overlay_dir / overlay_source.name)
    if patches_dir:
        for patch_path in spec.patches:
            patch_source = pathlib.Path(patch_path)
            shutil.copy2(patch_source, patches_dir / patch_source.name)

    _write_text(version_root / "MODULE.bazel", _module_content(spec))

    metadata = _metadata_payload(existing_metadata, spec)
    _write_json(metadata_path, metadata)

    source_integrity = _integrity(_download(_resolve_source(spec).url))
    source = _source_payload(spec, overlay_dir, patches_dir, source_integrity)
    _write_json(version_root / "source.json", source)
    return version_root


def _load_spec(input_path: str | None) -> ModuleSpec:
    if input_path:
        with open(input_path, encoding="utf-8") as handle:
            return ModuleSpec.from_dict(json.load(handle))
    return spec_from_user_input()


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--registry",
        default=".",
        help="Pfad zum Registry-Root (Standard: aktuelles Verzeichnis).",
    )
    parser.add_argument(
        "--input",
        help="JSON-Datei mit Moduleingaben statt interaktiver Abfrage.",
    )
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    registry_root = pathlib.Path(args.registry).resolve()
    spec = _load_spec(args.input)
    version_root = create_or_update_module(spec, registry_root)
    print(f"Modul {spec.name}@{spec.version} wurde unter {version_root} geschrieben.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
