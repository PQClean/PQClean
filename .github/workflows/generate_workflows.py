#!/usr/bin/env python3
import sys
import difflib

from pathlib import Path

import jinja2


def render_workflow(scheme_path):
    result = 0
    scheme_name = scheme_path.name
    scheme_type = str(scheme_path.parent.name).split("_")[1]
    target_name = f"{scheme_type}_{scheme_name}.yml"

    tpl = TPL_ENV.get_template("template.yml.j2")
    new_contents = tpl.render(scheme_type=scheme_type, scheme_name=scheme_name)
    if not TEST_MODE:
        with open(target_name, "w") as f:
            f.write(new_contents)
        with open("BADGES.md", "a") as f:
            f.write(
                f"![Test {scheme_name}]"
                f"(https://github.com/PQClean/PQClean/workflows/Test%20{scheme_name}/badge.svg?branch=master)\n")
    else:
        if (Path(".") / target_name).exists():
            with open(target_name, "r") as f:
                file_contents = f.read()
        else:
            file_contents = ""

        if file_contents != new_contents:
            sys.stdout.writelines(difflib.unified_diff(
                file_contents.splitlines(keepends=True), new_contents.splitlines(keepends=True),
                fromfile=f"old/{target_name}", tofile=f"new/{target_name}")
            )
            result = 1

    return result


if __name__ == "__main__":
    TEST_MODE = len(sys.argv) > 1 and sys.argv[1] == "test"

    loader = jinja2.FileSystemLoader(".")
    TPL_ENV = jinja2.Environment(
        loader=loader, variable_start_string="{-", variable_end_string="-}"
    )

    if not TEST_MODE:
        with open("BADGES.md", "w") as f:
            f.truncate()

    with open("template.yml.j2") as f:
        TEMPLATE = f.read()
    root = Path("../..")

    paths = root.glob("crypto_*/*")
    for path in sorted(paths):
        result = render_workflow(path)

    sys.exit(result)
