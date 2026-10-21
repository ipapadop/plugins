# Documenting skill provenance

## What this skill does

Creates evidence-based skill READMEs with dependency, source, authorship, version, and release-history information. Unknown facts remain `Not documented`.

## How it works

### Trigger conditions

Use when creating or updating a skill README that needs provenance, dependencies, sources, authors, semantic versions, or release history.

### Workflow

1. Inspect the target skill's files, Git history, current diff, and existing README.
2. Separate confirmed facts from unknowns. Preserve user-owned work and existing structure.
3. Propose corrections and release details for approval before recording them.
4. Update the README and verify evidence, completeness, and release order. An unchanged rerun does not create a release.

## Dependencies

### Scripts and runtimes

None for the skill itself. The agent needs file-reading and editing tools.

### Packages

None.

### Shell and operating-system commands

Git, or equivalent access to repository history and diffs. If history is shallow or unavailable, report the limitation.

### External tools and services

None required. Access to cited sources can help verify their revisions.

### Other skills

None required.

## Sources

| Source | Link or local path | Commit/tag | Version/release | Local-source description |
| --- | --- | --- | --- | --- |
| Original skill implementation | [SKILL.md](SKILL.md) | `758f20d6c70af28e6450a65377d6fee47088fd46` | Not documented | Evidence for the workflow first recorded in this repository. |
| README template | [assets/README.template.md](assets/README.template.md) | `758f20d6c70af28e6450a65377d6fee47088fd46` | Not documented | Defines the sections and unknown-fact placeholders for a new README. |

External sources used to create the original skill: Not documented.

## Version

Not documented. No independent skill release version is established in the inspected history. The plugin version is maintained separately in its manifests.

## Authors

- Yiannis Papadopoulos: author of the initial repository commit containing this skill.
- GitHub: recorded committer of that commit.

## Release notes

Newest releases appear first.

### Not documented

- Date: Not documented.
- Changes: No independent skill releases are established in the inspected history. The initial implementation was committed on 2026-09-01; this date is not evidence of a skill release.
