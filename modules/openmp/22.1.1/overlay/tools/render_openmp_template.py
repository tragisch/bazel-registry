#!/usr/bin/env python3

# This file is licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

"""Small renderer for libomp's CMake-style header templates."""

import argparse
import re
from pathlib import Path


def parse_defines(raw_defines):
    defines = {}
    for item in raw_defines:
        key, value = item.split("=", 1)
        defines[key] = value
    return defines


def truthy(value):
    return value not in ("0", "", "OFF", "FALSE", "False", "false", "None")


def render_line(line, defines):
    cmakedefine01 = re.match(r"^(\s*)#cmakedefine01\s+([A-Za-z0-9_]+)\s*$", line)
    if cmakedefine01:
        indent, name = cmakedefine01.groups()
        return f"{indent}#define {name} {1 if truthy(defines.get(name, '0')) else 0}"

    cmakedefine = re.match(r"^(\s*)#cmakedefine\s+([A-Za-z0-9_]+)(.*)$", line)
    if cmakedefine:
        indent, name, suffix = cmakedefine.groups()
        value = defines.get(name, "")
        if truthy(value):
            payload = suffix.strip() or value
            return f"{indent}#define {name} {payload}".rstrip()
        return f"{indent}/* #undef {name} */"

    rendered = line
    for key, value in defines.items():
        rendered = rendered.replace(f"@{key}@", value)
    return rendered


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--template", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--define", action="append", default=[])
    args = parser.parse_args()

    defines = parse_defines(args.define)
    template = Path(args.template)
    output = Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)

    rendered_lines = [render_line(line.rstrip("\n"), defines) for line in template.read_text().splitlines()]
    output.write_text("\n".join(rendered_lines) + "\n")


if __name__ == "__main__":
    main()
