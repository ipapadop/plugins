#!/usr/bin/env python3
"""Checks the plugins before merge.

Checks each skill's metadata against the Codex limits, and checks that a
change to any skill comes with a higher version of its plugin.

Usage: check-plugin.py [--base REF]

With --base, compares each plugin version with the version at the merge base
of REF and HEAD. Without it, checks only the skill metadata.
"""

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
PLUGINS = Path("plugins")
MANIFEST = Path(".claude-plugin/plugin.json")

# Codex shows at most 1024 characters of a description and truncates the rest.
MAX_DESCRIPTION = 1024
MAX_NAME = 64
# Codex skill-creator guideline for interface.short_description.
SHORT_DESCRIPTION = range(25, 65)

FRONT_MATTER = re.compile(r"\A---\n(.*?)\n---\n", re.DOTALL)


def git(*args):
    return subprocess.run(
        ["git", *args], cwd=ROOT, capture_output=True, text=True
    )


def check_skill(skill_dir):
    """Returns a list of problems with one skill's metadata."""
    match = FRONT_MATTER.match((skill_dir / "SKILL.md").read_text())
    front = yaml.safe_load(match.group(1)) if match else None
    if not isinstance(front, dict):
        return ["SKILL.md has no YAML front matter"]

    problems = []
    name = str(front.get("name") or "")
    description = str(front.get("description") or "")
    if name != skill_dir.name:
        problems.append(f"name {name!r} does not match directory {skill_dir.name!r}")
    if len(name) > MAX_NAME:
        problems.append(f"name has {len(name)} characters, maximum {MAX_NAME}")
    if not description:
        problems.append("description is empty")
    if len(description) > MAX_DESCRIPTION:
        problems.append(
            f"description has {len(description)} characters, maximum {MAX_DESCRIPTION}"
        )

    openai_yaml = skill_dir / "agents/openai.yaml"
    if openai_yaml.exists():
        interface = (yaml.safe_load(openai_yaml.read_text()) or {}).get("interface")
        short = str((interface or {}).get("short_description") or "")
        if len(short) not in SHORT_DESCRIPTION:
            problems.append(
                f"short_description has {len(short)} characters, expected "
                f"{SHORT_DESCRIPTION.start}-{SHORT_DESCRIPTION.stop - 1}"
            )
    return problems


def version_tuple(version):
    return tuple(int(part) for part in version.split("."))


def check_version(plugin, merge_base, base):
    """Returns a problem if a plugin's skills changed since base without a version bump.

    plugin is the plugin directory relative to ROOT.
    """
    manifest = plugin / MANIFEST
    diff = git("diff", "--name-only", merge_base, "--", str(plugin / "skills"))
    if diff.returncode != 0:
        return f"cannot compare {plugin} with {base}: {diff.stderr.strip()}"
    changed = diff.stdout.split()
    if not changed:
        return None
    old = git("show", f"{merge_base}:{manifest}")
    if old.returncode != 0:
        print(f"Version check skipped: {manifest} does not exist at {base}.")
        return None
    old_version = json.loads(old.stdout)["version"]
    new_version = json.loads((ROOT / manifest).read_text())["version"]
    if version_tuple(new_version) <= version_tuple(old_version):
        return (
            f"{len(changed)} skill file(s) changed but {manifest} version is "
            f"{new_version}, not higher than {old_version} at {base}"
        )
    return None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base", help="git ref to compare the plugin version with")
    args = parser.parse_args()

    failed = False
    plugins = sorted(
        p.relative_to(ROOT)
        for p in (ROOT / PLUGINS).iterdir()
        if (p / MANIFEST).is_file()
    )
    for plugin in plugins:
        skills = ROOT / plugin / "skills"
        for skill_dir in sorted(p for p in skills.iterdir() if p.is_dir()):
            for problem in check_skill(skill_dir):
                print(f"{plugin.name}/{skill_dir.name}: {problem}")
                failed = True
    if args.base:
        merge_base = git("merge-base", args.base, "HEAD")
        if merge_base.returncode != 0:
            print(
                f"cannot find the merge base of {args.base} and HEAD: "
                f"{merge_base.stderr.strip()}"
            )
            failed = True
        else:
            for plugin in plugins:
                problem = check_version(plugin, merge_base.stdout.strip(), args.base)
                if problem:
                    print(problem)
                    failed = True
    if not failed:
        print("OK")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
