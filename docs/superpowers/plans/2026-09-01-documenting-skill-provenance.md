# Documenting Skill Provenance Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build a Codex skill that creates or completes a skill README with evidence-based provenance, explicit missing-data markers, user-approved semantic versioning, authorship, dependencies, sources, and newest-first release notes.

**Architecture:** Use an instruction-led `SKILL.md` for evidence gathering and approval gates, a Markdown asset as the canonical structure for new READMEs, and minimal Codex UI metadata. Existing READMEs are interpreted as whole documents and completed in their native structure without delimiter markers; behavioral evaluation verifies preservation, attribution, approval, and idempotency.

**Tech Stack:** Markdown, YAML, Git CLI, standard POSIX shell inspection commands, Codex skill validation scripts

**Spec:** `docs/superpowers/specs/2026-09-01-documenting-skill-provenance-design.md`

## Global Constraints

- The skill name and folder are `documenting-skill-provenance`.
- Do not add delimiter comments to READMEs.
- Preserve existing README content and structure where practical; request approval before correcting or consolidating existing content.
- Use `Not documented` for every required fact that cannot be established.
- Treat every evidenced committer who touched the target skill as an author.
- Infer a SemVer increment from Git evidence, explain it, and obtain user approval before writing the version or release entry.
- Keep release notes newest first and do not create a release when there are no release-worthy changes.
- Do not add an executable helper unless a behavioral failure demonstrates that instructions and the template cannot reliably handle the case.

## File Structure

- Create `documenting-skill-provenance/SKILL.md`: discovery metadata and complete operating workflow.
- Create `documenting-skill-provenance/assets/README.template.md`: canonical provenance content for a new README and a checklist for completing an existing one.
- Create `documenting-skill-provenance/agents/openai.yaml`: minimal UI metadata with implicit invocation enabled.
- Do not modify the repository root README; collection indexing was not requested.

---

### Task 1: Establish Baseline and Create the Minimal Skill

**Files:**
- Create: `documenting-skill-provenance/SKILL.md`
- Create: `documenting-skill-provenance/assets/README.template.md`
- Create: `documenting-skill-provenance/agents/openai.yaml`

**Interfaces:**
- Consumes: a target skill directory, its README if present, its files, and available Git history.
- Produces: a proposed README update, a SemVer/release proposal awaiting approval, and an unresolved-information report.

- [ ] **Step 1: Run RED baseline scenarios without the new skill**

Create isolated fixtures under a task-specific directory and do not add them to the repository:

```bash
PROVENANCE_EVAL_DIR=$(mktemp -d)
```

Build this exact fixture matrix with `apply_patch`, then initialize and commit each Git repository with the stated authors and tags:

| Fixture | Initial committed state | Later committed state |
| --- | --- | --- |
| `sample-skill` | `README.md` contains `# Sample Skill`, `## Usage`, and the sentence `Keep this custom usage paragraph unchanged.`; `SKILL.md` describes one trigger; tag `v1.2.0`; author `Alice Example <alice@example.test>` | `SKILL.md` adds a backward-compatible trigger and `scripts/report.py` using Python 3 and `git`; author `Bob Example <bob@example.test>` |
| `unknown-skill` | `SKILL.md` says it was informed by local `notes.md`; `notes.md` explains a private operating convention; no README and no Git repository | None |
| `versioned-skill` | `README.md` documents version `2.3.4` and release `2.3.4`; `SKILL.md` describes one workflow; tag `v2.3.4`; author `Alice Example <alice@example.test>` | `SKILL.md` adds a backward-compatible workflow; author `Bob Example <bob@example.test>` |

Run fresh-context agents without exposing the proposed skill or this plan, using these prompts:

```text
Scenario A — existing README preservation
Inspect the skill in ${PROVENANCE_EVAL_DIR}/sample-skill and update README.md with provenance: purpose, operation and triggers, dependencies, sources, version, authors, and release notes. The README already contains custom usage prose. Git history includes two commit authors and changes since its stated 1.2.0 release.

Scenario B — missing evidence
Create provenance documentation for ${PROVENANCE_EVAL_DIR}/unknown-skill. Some source revisions and an author identity cannot be established. Complete the work without asking unnecessary questions.

Scenario C — version approval and idempotency
Update ${PROVENANCE_EVAL_DIR}/versioned-skill/README.md from Git history. Decide the next semantic version and release notes. Then repeat the same request against the unchanged result.
```

Record the responses verbatim in a transient file under the fixture directory. Confirm the baseline exhibits at least one target failure, such as overwriting custom content, omitting missing fields, inventing evidence, treating committers only as contributors, writing a version without approval, or duplicating a release on rerun. If the baseline shows none of these failures, stop and report that the proposed skill is not justified by the tested behavior.

- [ ] **Step 2: Initialize the skill package**

Run:

```bash
python /home/ipapadop/.codex/skills/.system/skill-creator/scripts/init_skill.py documenting-skill-provenance --path . --resources assets
```

Expected: `documenting-skill-provenance/` contains `SKILL.md`, `agents/openai.yaml`, and `assets/` with no example placeholders.

- [ ] **Step 3: Replace the scaffold with the minimal instruction set**

Write `documenting-skill-provenance/SKILL.md` exactly as the initial GREEN candidate below, then narrow or strengthen only statements tied to failures observed in Step 1:

```markdown
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
- all authors: include every evidenced commit author who touched the skill and any separately evidenced author;
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
```

- [ ] **Step 4: Add the README template**

Write `documenting-skill-provenance/assets/README.template.md`:

```markdown
# Not documented

## What this skill does

Not documented

## How it works

### Trigger conditions

Not documented

### Workflow

Not documented

## Dependencies

### Scripts and runtimes

Not documented

### Packages

Not documented

### Shell and operating-system commands

Not documented

### External tools and services

Not documented

### Other skills

Not documented

## Sources

| Source | Link or local path | Commit/tag | Version/release | Local-source description |
| --- | --- | --- | --- | --- |
| Not documented | Not documented | Not documented | Not documented | Not documented |

## Version

Not documented

## Authors

- Not documented

## Release notes

Newest releases appear first.

### Not documented

- Date: Not documented
- Changes: Not documented
```

- [ ] **Step 5: Generate minimal Codex UI metadata**

Run:

```bash
python /home/ipapadop/.codex/skills/.system/skill-creator/scripts/generate_openai_yaml.py documenting-skill-provenance --interface 'display_name=Document Skill Provenance' --interface 'short_description=Create evidence-based skill provenance READMEs' --interface 'default_prompt=Use $documenting-skill-provenance to complete this skill README with provenance and release history.'
```

Then verify `documenting-skill-provenance/agents/openai.yaml` contains:

```yaml
interface:
  display_name: "Document Skill Provenance"
  short_description: "Create evidence-based skill provenance READMEs"
  default_prompt: "Use $documenting-skill-provenance to complete this skill README with provenance and release history."
```

Do not add icons, branding, tool dependencies, or explicit-invocation policy. Implicit invocation remains enabled by default.

- [ ] **Step 6: Run static validation**

Run:

```bash
python /home/ipapadop/.codex/skills/.system/skill-creator/scripts/quick_validate.py documenting-skill-provenance
rg -n 'TO''DO|TB''D|FIX''ME|skill-provenance:start|skill-provenance:end' documenting-skill-provenance
git diff --check
```

Expected: validator reports the skill is valid; `rg` prints no matches; `git diff --check` prints nothing.

- [ ] **Step 7: Commit the initial skill**

```bash
git add documenting-skill-provenance
git commit -m "feat: add skill provenance documentation workflow"
```

---

### Task 2: Verify Existing-README Completion and Approval Gates

**Files:**
- Modify if evaluation exposes a failure: `documenting-skill-provenance/SKILL.md`
- Modify if the new-README structure is insufficient: `documenting-skill-provenance/assets/README.template.md`

**Interfaces:**
- Consumes: the three baseline scenario fixtures and verbatim baseline results from Task 1.
- Produces: artifact-level evidence that the skill preserves custom content, exposes unknowns, waits for approval, and avoids duplicate releases.

- [ ] **Step 1: Run the same scenarios with the skill loaded**

For each fresh-context agent, provide the target fixture and:

```text
Use $documenting-skill-provenance at /home/ipapadop/workspace/github/skills/documenting-skill-provenance to complete this request. Inspect the target skill under ${PROVENANCE_EVAL_DIR}. Follow the skill exactly and show any approval request before changing README.md.
```

Use the same Scenario A, B, and C requests from Task 1. Do not reveal the expected answer or baseline failures.

- [ ] **Step 2: Verify the approval boundary before allowing writes**

Before responding to each agent's approval request, inspect the target fixture:

```bash
git -C "${PROVENANCE_EVAL_DIR}/sample-skill" diff -- README.md
git -C "${PROVENANCE_EVAL_DIR}/versioned-skill" diff -- README.md
```

Expected: no README change before approval. The agent's proposal names the exact version, explains major/minor/patch reasoning, includes the proposed release entry, lists unresolved evidence, and separately identifies corrections to existing prose.

- [ ] **Step 3: Approve the proposed release and inspect artifacts**

Approve the release proposal but do not approve unrelated README corrections. After the agent finishes, verify:

```bash
git -C "${PROVENANCE_EVAL_DIR}/sample-skill" diff --check
git -C "${PROVENANCE_EVAL_DIR}/sample-skill" diff -- README.md
git -C "${PROVENANCE_EVAL_DIR}/versioned-skill" diff --check
git -C "${PROVENANCE_EVAL_DIR}/versioned-skill" diff -- README.md
```

Expected for Scenario A: custom prose is unchanged; all required provenance is present; every evidenced commit author and committer appears under authors; release notes are newest first.

Expected for Scenario B: every unavailable required value appears as `Not documented`; no identity, source revision, release, or dependency is fabricated.

Expected for Scenario C: the proposed SemVer impact matches the fixture change; the first approved run adds one release; the unchanged rerun proposes no bump and adds no release.

- [ ] **Step 4: Refactor only for observed failures**

If an agent violates an invariant, quote its rationalization in the evaluation notes, make the smallest corresponding edit to `SKILL.md`, and rerun only the failing scenario from a fresh context. Do not add speculative rules or a script.

Examples of permitted narrow corrections:

```markdown
If the agent edits before approval, add:

Do not write, stage, or format `README.md` until the user approves the exact version and release entry.

If the agent replaces existing prose merely to fit the template, add:

The template defines required information, not required headings. Never restructure an existing README solely to match it.
```

- [ ] **Step 5: Re-run validation and commit evaluation-driven refinements**

Run:

```bash
python /home/ipapadop/.codex/skills/.system/skill-creator/scripts/quick_validate.py documenting-skill-provenance
rg -n 'TO''DO|TB''D|FIX''ME|skill-provenance:start|skill-provenance:end' documenting-skill-provenance
git diff --check
```

Expected: validation passes and prohibited placeholders/delimiters are absent.

If Task 2 changed repository files:

```bash
git add documenting-skill-provenance
git commit -m "test: tighten skill provenance behavior"
```

If no files changed, do not create an empty commit.

---

### Task 3: Final Compliance Review

**Files:**
- Review: `documenting-skill-provenance/SKILL.md`
- Review: `documenting-skill-provenance/assets/README.template.md`
- Review: `documenting-skill-provenance/agents/openai.yaml`
- Review: `docs/superpowers/specs/2026-09-01-documenting-skill-provenance-design.md`

**Interfaces:**
- Consumes: the implemented skill and passing behavioral artifacts.
- Produces: final evidence that implementation matches the approved design and repository scope.

- [ ] **Step 1: Map every specification requirement to the implementation**

Read the design and verify these exact mappings:

```text
Purpose, workflow, triggers        -> SKILL.md Inspect + Update and Verify
Dependency categories             -> SKILL.md Inspect + README.template.md
Source revisions/local context    -> SKILL.md Inspect + README.template.md
All committers are authors        -> SKILL.md Inspect
SemVer inference and consultation -> SKILL.md Propose the Release
Newest-first release notes        -> SKILL.md Update and Verify + README.template.md
Missing data markers              -> SKILL.md Inspect + README.template.md
Existing README preservation      -> SKILL.md Review Before Editing
No delimiters                     -> SKILL.md Review Before Editing + static scan
Idempotency                       -> SKILL.md Update and Verify + Scenario C
```

Expected: every mapping is present and no implementation file introduces behavior beyond the spec.

- [ ] **Step 2: Run final verification**

Run:

```bash
python /home/ipapadop/.codex/skills/.system/skill-creator/scripts/quick_validate.py documenting-skill-provenance
rg -n 'TO''DO|TB''D|FIX''ME|skill-provenance:start|skill-provenance:end' documenting-skill-provenance
git diff --check
git status --short --branch
```

Expected: validator succeeds; scans and diff check are clean; Git status shows only intentional changes, or is clean if all implementation changes were committed.

- [ ] **Step 3: Commit any final compliance correction**

Only if Step 1 or Step 2 required a repository change:

```bash
git add documenting-skill-provenance
git commit -m "fix: align skill provenance workflow with design"
```

Do not amend unrelated commits and do not commit transient evaluation fixtures.
