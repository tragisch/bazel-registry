import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from select_presubmits import select


class SelectPresubmitsTests(unittest.TestCase):
    def setUp(self):
        workspace = tempfile.TemporaryDirectory()
        self.addCleanup(workspace.cleanup)
        current_directory = Path.cwd()
        os.chdir(workspace.name)
        self.addCleanup(os.chdir, current_directory)
        self.git("init", "--quiet")

    def git(self, *args):
        return subprocess.check_output(["git", *args], text=True).strip()

    def write(self, path, text=""):
        file = Path(path)
        file.parent.mkdir(parents=True, exist_ok=True)
        file.write_text(text)

    def add_version(self, version, presubmit=True):
        directory = f"modules/example/{version}"
        self.write(f"{directory}/MODULE.bazel")
        self.write(f"{directory}/source.json", "{}")
        if presubmit:
            self.write(f"{directory}/presubmit.yml", """\
matrix:
  platform: [ubuntu-latest, macos-latest]
tasks:
  smoke:
    build_targets: ["@example//:library"]
    test_targets: ["@example//:smoke"]
""")

    def commit(self):
        self.git("add", ".")
        self.git("-c", "user.name=Test", "-c", "user.email=test@example.com",
                 "commit", "--quiet", "-m", "Registry fixture")
        return self.git("rev-parse", "HEAD")

    def test_new_version_without_presubmit_fails_even_on_full_run(self):
        self.add_version("1.0", presubmit=False)
        base = self.commit()
        self.add_version("1.1", presubmit=False)
        self.write(".github/workflows/test-registry.yml")
        with self.assertRaisesRegex(ValueError, r"New registry versions require presubmit.yml: example@1\.1"):
            select(base, self.commit(), [{
                "name": "example", "version": "1.1", "target": "@example//:legacy",
                "command": "build",
            }])

    def test_new_version_runs_its_own_tasks_on_both_platforms(self):
        self.add_version("1.0")
        base = self.commit()
        self.add_version("1.1")
        matrix, count = select(base, self.commit(), [])
        self.assertEqual(count, 1)
        self.assertEqual([row["os"] for row in matrix["include"]], ["ubuntu-latest", "macos-latest"])
        for row in matrix["include"]:
            self.assertEqual(row["module"]["version"], "1.1")
            self.assertEqual(row["module"]["commands"], [
                {"command": "build", "targets": ["@example//:library"]},
                {"command": "test", "targets": ["@example//:smoke"]},
            ])

    def test_existing_legacy_version_keeps_its_matrix_row(self):
        self.add_version("1.0", presubmit=False)
        self.write("modules/example/1.0/presubmit.yml", "legacy BCR configuration")
        base = self.commit()
        self.write("modules/example/metadata.json", json.dumps({"versions": ["1.0"]}))
        matrix, count = select(base, self.commit(), [{
            "name": "example", "version": "1.0", "target": "@example//:legacy",
            "command": "build", "os": ["ubuntu-latest"],
        }])
        self.assertEqual(count, 1)
        self.assertEqual(len(matrix["include"]), 1)
        self.assertEqual(matrix["include"][0]["module"]["commands"], [
            {"command": "build", "targets": ["@example//:legacy"]},
        ])

    def test_changed_overlay_runs_only_its_version_from_the_committed_config(self):
        self.add_version("1.0")
        self.add_version("1.1")
        base = self.commit()
        self.write("modules/example/1.1/overlay/BUILD.bazel")
        head = self.commit()
        self.write("modules/example/1.1/presubmit.yml", "broken in the worktree")
        matrix, count = select(base, head, [])
        self.assertEqual(count, 1)
        self.assertEqual({row["module"]["version"] for row in matrix["include"]}, {"1.1"})
        self.assertEqual({row["os"] for row in matrix["include"]}, {"ubuntu-latest", "macos-latest"})
        self.assertEqual(matrix["include"][0]["module"]["commands"][0]["targets"], ["@example//:library"])

    def test_changed_metadata_selects_both_versions(self):
        self.add_version("1.0")
        self.add_version("1.1")
        base = self.commit()
        self.write("modules/example/metadata.json", '{"versions": ["1.0", "1.1"]}')
        matrix, count = select(base, self.commit(), [])
        self.assertEqual(count, 2)
        self.assertEqual({row["module"]["version"] for row in matrix["include"]}, {"1.0", "1.1"})

    def test_removing_an_active_presubmit_fails(self):
        self.add_version("1.0")
        base = self.commit()
        Path("modules/example/1.0/presubmit.yml").unlink()
        with self.assertRaisesRegex(ValueError, r"Existing registry versions lost presubmit.yml: example@1\.0"):
            select(base, self.commit(), [])
