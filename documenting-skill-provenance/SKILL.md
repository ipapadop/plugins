---
name: documenting-skill-provenance
description: Use when creating or updating a skill README that needs provenance, dependency, source, authorship, semantic-version, or release-history information.
---

# Documenting Skill Provenance

## Outcome

Create or complete one target skill's `README.md` from inspected evidence. Preserve existing prose and organization, expose unknown facts, and obtain approval before changing existing claims or recording a release.

## Inspect

Read the target skill's `SKILL.md`, `agents/`, scripts, references, assets, and README. Inspect available Git history and the diff since the last documented release. Use the README as a consistency check, not as proof of its own claims.

Establish:

- what the skill does and how it works, including trigger conditions;
- runtimes, packages, scripts, shell commands, external tools/services, and other skills it actually uses;
- sources used to create it, with names, URLs or local paths, repository commit hashes/tags, and published versions/releases where applicable;
- a one-sentence explanation for each local source;
- all authors: include every evidenced commit author or committer who touched the skill and any separately evidenced author;
- changes since the last release and their compatibility impact.

Do not invent evidence. Write `Not documented` wherever a required fact cannot be established. Separate confirmed dependencies from unresolved candidates.

## Review Before Editing

Read an existing README as a whole. Recognize equivalent headings rather than requiring a fixed layout. Preserve content and structure where practical. Report stale or contradictory claims and ambiguous or duplicate provenance sections; request approval before correcting, removing, moving, or consolidating them.

Use [assets/README.template.md](assets/README.template.md) as the starting structure only when no README exists. For an existing README, fill missing information with the smallest coherent edits and add only absent sections. Never add provenance delimiter comments.

## Propose the Release

Use Git history/diffs as the primary release evidence and the README as a secondary cross-check. Propose:

- `major` for incompatible changes to behavior, invocation, inputs, outputs, or required dependencies;
- `minor` for backward-compatible capabilities or meaningful workflows;
- `patch` for backward-compatible corrections, clarifications, or internal improvements.

If no version exists, propose `0.1.0`. Explain the evidence and ask the user to confirm the exact version and release-note entry before writing either. If there are no release-worthy changes, do not bump the version or add a release.

Also request approval for proposed corrections to existing README content. Do not treat approval of the release as approval of unrelated corrections.

## Update and Verify

After approval, create or update `README.md`. It must contain:

1. What the skill does.
2. How it works and triggers.
3. Dependencies by category.
4. Sources and their available revisions/releases.
5. Current version.
6. Authors.
7. Release notes ordered newest first, with version, date, and changes.

Re-read the result. Verify existing content was preserved except for approved corrections, every required section is present, unknowns say `Not documented`, authors match evidence, and release order is newest first. Report unresolved information. An unchanged rerun must not create another release.

## Stop Conditions

Stop and request direction when the target is unclear, Git evidence cannot distinguish the proposed release, or provenance sections are ambiguous enough that completing them would rewrite existing meaning. Report shallow or unavailable Git history and mark affected facts `Not documented`.
