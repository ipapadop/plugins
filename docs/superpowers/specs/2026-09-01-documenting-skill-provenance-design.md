# Documenting Skill Provenance Design

## Purpose

Create a Codex skill named `documenting-skill-provenance` that creates or updates a target skill's `README.md` with evidence-based provenance. The README must explain what the skill does, how it works and is triggered, what it depends on, which sources informed it, its version and authors, and its release history.

The skill must preserve existing custom README content. It reviews that content for stale or contradictory claims, but changes it only after user approval.

## Package Structure

The skill contains:

- `SKILL.md`, defining trigger conditions, inspection workflow, evidence requirements, approval gates, and update rules.
- `assets/README.template.md`, defining the canonical managed provenance block.
- `agents/openai.yaml`, providing minimal Codex UI metadata while retaining automatic discovery.

No executable helper is included initially. Dependency discovery, source assessment, and semantic-version classification require judgment, so an instruction-led workflow is the smallest appropriate design. A script should be added only if behavioral testing demonstrates a specific reliability problem that deterministic automation would solve.

## README Contract

The skill manages one block delimited by:

```markdown
<!-- skill-provenance:start -->
...
<!-- skill-provenance:end -->
```

Content outside this block is preserved byte-for-byte unless the user separately approves a correction. The managed block contains:

1. A description of what the skill does.
2. How it works, including its trigger conditions.
3. Dependencies, categorized where applicable as:
   - scripts and runtimes;
   - software packages;
   - shell or operating-system commands;
   - external tools or services;
   - other skills.
4. Sources used to create the skill, including:
   - source name;
   - URL or local path;
   - repository commit hash or tag when applicable;
   - published version or release number when applicable;
   - a short explanation of every local source.
5. Current semantic version.
6. Authors. Anyone represented in a commit that modifies the skill is an author, alongside authors explicitly supplied with evidence.
7. Release notes ordered newest first. Each entry contains its version, date, and summarized changes.

Every required fact that cannot be established is retained visibly as `Not documented`; required sections are never silently omitted.

## Inspection and Update Flow

The skill operates on one target skill directory at a time:

1. Read the target's `SKILL.md`, UI metadata, scripts, references, assets, and existing README.
2. Inspect Git history and the diff since the last documented release. Use the existing README as a secondary consistency check.
3. Inventory confirmed dependencies and sources. Report unresolved candidates separately rather than presenting them as facts.
4. Derive authors from commits that touched the skill and merge them with evidenced, explicitly supplied authors.
5. Review preserved README content for contradictions or stale statements.
6. Report missing information, evidence limitations, and proposed corrections.
7. Infer whether the accumulated changes require a major, minor, or patch increment. Explain the compatibility and behavior evidence supporting that proposal.
8. Ask the user to approve the proposed semantic version, release entry, and any corrections outside the managed block.
9. After approval, create or replace only the managed provenance block and apply only separately approved corrections.
10. Re-read the result, verify ordering and preservation, and report unresolved `Not documented` fields.

If no version exists, the skill proposes `0.1.0` and requires confirmation. If there are no release-worthy changes, it does not create a version bump or release entry.

## Semantic-Version Rules

The skill proposes:

- `major` when changes remove or incompatibly alter documented behavior, invocation expectations, required inputs, outputs, or dependencies;
- `minor` when backward-compatible capabilities or meaningful workflows are added;
- `patch` for backward-compatible corrections, clarifications, or internal improvements.

These rules guide a proposal, not an autonomous decision. The README is not updated until the user confirms the version and release notes. Ambiguous cases must be surfaced rather than resolved silently.

## Failure and Ambiguity Handling

- Unavailable, shallow, or ambiguous Git history is reported. Any affected provenance is marked `Not documented`.
- Uncertain dependency usage is separated from confirmed dependencies.
- Missing authors, source revisions, and release dates are never invented.
- A malformed or duplicated managed block causes the skill to stop and request direction before rewriting it.
- Contradictions in preserved README content are shown to the user with the supporting evidence; corrections require approval.
- Existing uncommitted work is treated as user-owned and must not be overwritten or attributed without evidence.

## Verification

The completed skill must pass the standard `quick_validate.py` check and behavioral scenarios covering:

- creating a README when none exists;
- updating a README while preserving custom content;
- explicit missing-information markers;
- dependency and source inventories;
- Git-derived authors and release changes;
- a reasoned SemVer proposal followed by mandatory user approval;
- newest-first release-note ordering;
- a malformed or duplicated managed block;
- idempotency, so rerunning without release-worthy changes creates no new release.

Behavioral evaluation should first establish baseline failures without the skill, then repeat the same scenarios with the skill and inspect the generated artifacts. Any additional instruction or automation must address an observed failure rather than a hypothetical one.

## Success Criteria

The work is complete when another Codex instance can use the skill to produce an accurate, reviewable provenance README; preserve unrelated content; expose unknown facts; propose and confirm semantic versioning; attribute all evidenced contributors as authors; and maintain newest-first release notes without duplicating entries on an unchanged rerun.
