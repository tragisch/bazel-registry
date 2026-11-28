"""Select registry presubmits for the changed module versions."""

import argparse
import json
import os
from pathlib import PurePosixPath
import subprocess
import sys

import yaml


RUNNERS = ("ubuntu-latest", "macos-latest")


def git_paths(*args):
    output = subprocess.check_output(["git", *args])
    return [PurePosixPath(os.fsdecode(path)) for path in output.split(b"\0") if path]


def version_files(paths, *names):
    return {
        (parts[1], parts[2]): path
        for path in paths
        if len(parts := path.parts) == 4
        and parts[0] == "modules"
        and parts[3] in names
    }


def presubmit_jobs(head, name, version, path):
    config = yaml.safe_load(subprocess.check_output(["git", "show", f"{head}:{path}"]))
    if not isinstance(config, dict) or set(config) - {"matrix", "tasks"}:
        raise ValueError(f"{path}: expected a top-level matrix and tasks")

    matrix = config.get("matrix")
    if not isinstance(matrix, dict) or set(matrix) != {"platform"}:
        raise ValueError(f"{path}: expected matrix.platform")
    platforms = matrix["platform"]
    if (
        not isinstance(platforms, list)
        or not platforms
        or any(runner not in RUNNERS for runner in platforms)
    ):
        raise ValueError(f"{path}: matrix.platform must select from {', '.join(RUNNERS)}")

    tasks = config.get("tasks")
    if not isinstance(tasks, dict) or not tasks:
        raise ValueError(f"{path}: expected at least one task")
    for task_id, task in tasks.items():
        if not isinstance(task, dict) or set(task) - {"name", "build_targets", "test_targets"}:
            raise ValueError(f"{path}: {task_id} must declare build_targets or test_targets")
        commands = []
        for command in ("build", "test"):
            field = f"{command}_targets"
            if field not in task:
                continue
            targets = task[field]
            if not isinstance(targets, list) or not targets or any(
                not isinstance(target, str)
                or not target.startswith(f"@{name}//")
                or any(character.isspace() for character in target)
                for target in targets
            ):
                raise ValueError(f"{path}: {task_id}.{field} must contain targets in @{name}//")
            commands.append({"command": command, "targets": targets})
        if not commands:
            raise ValueError(f"{path}: {task_id} must declare build_targets or test_targets")
        yield {
            "name": name,
            "version": version,
            "label": f"{name}@{version} ({task_id})",
            "commands": commands,
            "os": list(dict.fromkeys(platforms)),
            "presubmit": True,
        }


def select(base, head, legacy):
    head_paths = set(git_paths("ls-tree", "-r", "--name-only", "-z", head, "modules"))
    current = set(version_files(head_paths, "MODULE.bazel", "source.json"))
    if base and base.strip("0"):
        base_paths = git_paths("ls-tree", "-r", "--name-only", "-z", base, "modules")
        changed = git_paths("diff", "--name-only", "--no-renames", "-z", base, head)
    else:
        base_paths = []
        changed = head_paths

    added = current - version_files(base_paths, "MODULE.bazel", "source.json").keys()
    configs = version_files(head_paths, "presubmit.yml")
    if missing := sorted(added - configs.keys()):
        entries = ", ".join(f"{name}@{version}" for name, version in missing)
        raise ValueError(f"New registry versions require presubmit.yml: {entries}")

    # Existing legacy rows remain authoritative until explicitly migrated.
    legacy_versions = {(entry["name"], entry["version"]) for entry in legacy} - added
    previous_presubmits = version_files(base_paths, "presubmit.yml")
    if removed := sorted((current & previous_presubmits.keys()) - configs.keys() - legacy_versions):
        entries = ", ".join(f"{name}@{version}" for name, version in removed)
        raise ValueError(f"Existing registry versions lost presubmit.yml: {entries}")

    run_all = not changed or any(
        path.parts[0] != "modules" and path != PurePosixPath("README.md") for path in changed
    )
    changed_modules = {
        path.parts[1] for path in changed if path.parts[0] == "modules" and len(path.parts) >= 3
    }
    changed_versions = {
        (parts[1], parts[2])
        for path in changed
        if len(parts := path.parts) >= 4 and parts[0] == "modules"
    }
    changed_metadata = {
        path.parts[1]
        for path in changed
        if len(path.parts) == 3 and path.parts[0] == "modules" and path.name == "metadata.json"
    }

    migrated = (current & configs.keys()) - legacy_versions
    covered_names = {name for name, _ in (legacy_versions | migrated)}
    uncovered = sorted(
        name for name in changed_modules - covered_names
        if PurePosixPath("modules", name, "metadata.json") in head_paths
    )
    if uncovered and not run_all:
        raise ValueError(f"Changed modules missing registry test configuration: {', '.join(uncovered)}")

    selected = []
    for entry in legacy:
        key = entry["name"], entry["version"]
        if key in legacy_versions and key in current and (run_all or entry["name"] in changed_modules):
            selected.append({
                "name": entry["name"],
                "version": entry["version"],
                "label": entry.get("label", entry["name"]),
                "commands": [{"command": entry["command"], "targets": entry["target"].split()}],
                "os": entry.get("os", RUNNERS),
            })
    for name, version in sorted(migrated):
        if run_all or (name, version) in changed_versions or name in changed_metadata:
            selected.extend(presubmit_jobs(head, name, version, configs[name, version]))

    matrix = {"include": [
        {"os": runner, "module": {key: value for key, value in entry.items() if key != "os"}}
        for entry in selected
        for runner in entry["os"]
    ]}
    return matrix, len(selected)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", required=True)
    parser.add_argument("--head", required=True)
    args = parser.parse_args()
    try:
        matrix, count = select(args.base, args.head, json.load(sys.stdin))
    except (OSError, ValueError, yaml.YAMLError, subprocess.CalledProcessError) as error:
        sys.exit(str(error))

    with open(os.environ["GITHUB_OUTPUT"], "a", encoding="utf-8") as output:
        print(f"matrix={json.dumps(matrix, separators=(',', ':'))}", file=output)
        print(f"count={count}", file=output)
    with open(os.environ["GITHUB_STEP_SUMMARY"], "a", encoding="utf-8") as summary:
        print("### Registry module selection", file=summary)
        for row in matrix["include"]:
            print(f'- {row["module"]["label"]} on {row["os"]}', file=summary)
        if not count:
            print("No registry module changes selected.", file=summary)


if __name__ == "__main__":
    main()
