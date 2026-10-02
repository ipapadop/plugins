# Simplified Technical English

## What this skill does

The skill makes an agent write short, direct technical text without filler. It applies the writing rules of ASD-STE100 Simplified Technical English (Issue 9) to documents, code comments, commit messages, and replies to technical requests. It does not enforce the STE approved-word dictionary. The agent uses the normal technical vocabulary of the subject.

## How it works

### Trigger conditions

The agent loads the skill when it writes or edits a document (README, specification, design document, procedure, runbook, API or user guide, report), a code comment, or a commit message. The agent also loads it when it replies to a technical request. The user does not have to mention style. The skill does not apply to fiction or marketing copy. If the user gives a house style, the agent follows that style and applies the skill where the style is silent. For code documentation, the `code-documentation-style` skill takes precedence if it is available.

### Workflow

1. The agent applies 11 core rules to all text. For example, the rules tell the agent to lead with the answer, cut filler, and keep sentences short.
2. The agent applies the rules for the type of text: procedures (imperative steps, one action in each step, safety instructions before the risky step), descriptions (one topic in each paragraph, no commands), or chat replies (answer first, state uncertainty once).
3. Before it sends the text, the agent checks the draft against a five-item list. The last item makes sure that no needed fact, step, or warning was removed.

## Dependencies

### Scripts and runtimes

None.

### Packages

None.

### Shell and operating-system commands

None.

### External tools and services

None.

### Other skills

None.

## Sources

| Source | Link or local path | Commit/tag | Version/release | Local-source description |
| --- | --- | --- | --- | --- |
| ASD-STE100 Simplified Technical English | https://www.asd-ste100.org/assets/files/ASD-STE100_ISSUE9.pdf | Not applicable | Issue 9, 2025-01-15 | Not applicable |

The skill paraphrases the writing rules in Part 1 of the specification. It does not copy the specification text or its dictionary. ASD owns the copyright of the specification.

## Version

1.0.1

## Authors

- Yiannis Papadopoulos

## Release notes

Newest releases appear first.

### 1.0.1

- Date: 2026-10-02
- Changes: Defers to the `code-documentation-style` skill for code documentation, so that the two skills do not give conflicting rules (for example, about contractions).

### 1.0.0

- Date: 2026-10-02
- Changes: Initial release. Applies the ASD-STE100 Issue 9 writing rules, without the approved-word dictionary, to documents, procedures, descriptions, code comments, commit messages, and chat replies.
