# plugins

Collection of opinionated agent skills for Codex, Claude Code, and other tools that support the Agent Skills format.

## Available skills

All skills are in one plugin, `ipapadop-skills`.

- `code-documentation-style`: makes agents write docstrings, API comments, code comments, and code READMEs in clear, compact language, based on the Google developer documentation style guide.
- `power-of-ten`: makes agents write, review, and refactor safety-critical C and C++ code with Gerard Holzmann's "Power of Ten" rules (NASA/JPL), with a C++ interpretation of each rule.
- `simplified-technical-english`: makes agents write short, direct technical text without filler, based on the writing rules of ASD-STE100 Simplified Technical English (Issue 9).
- `skill-provenance`: creates evidence-based skill documentation with provenance, dependency, authorship, version, and release-history details.

## Evaluate the skills

Each skill has cases in `evals/evals.json`. Each case includes a prompt, expected behavior, and assertions about the result.

Run a case in a fresh agent context with the target `SKILL.md` available. Save generated artifacts outside the repository. Compare the result against each assertion and record supporting evidence separately from the case definitions.

For code transformations, execute the original and proposed code on representative and edge-case inputs. Check outputs, types, exceptions, ordering, and side effects. Performance claims require before/after measurements on the same workload and runtime.

For prose and provenance cases, inspect factual accuracy, required wording, edit scope, and whether approval boundaries were respected. The cases are evaluation inputs, not evidence of passing behavior. To measure skill benefit, compare against an independent run without the skill.

## Install in Codex

### Install one skill

Ask Codex:

```text
Use $skill-installer to install plugins/ipapadop-skills/skills/skill-provenance from ipapadop/plugins.
```

The skill becomes available on the next turn.

### Install all skills

Register this repository as a marketplace and install the plugin:

```bash
codex plugin marketplace add ipapadop/plugins
codex plugin add ipapadop-skills@ipapadop
```

Start a new Codex thread after installation so Codex discovers the plugin's skills.

## Install in Claude Code

Run these commands inside Claude Code:

```text
/plugin marketplace add ipapadop/plugins
/plugin install ipapadop-skills@ipapadop
```

If the installation summary requests it, run `/reload-plugins`. Each skill is available as `/ipapadop-skills:<skill>`, for example `/ipapadop-skills:skill-provenance`.

## Install in Gemini CLI

Install all skills:

```bash
gemini skills install https://github.com/ipapadop/plugins.git \
  --path plugins/ipapadop-skills/skills
```

To install one skill, add its directory to the path, for example `--path plugins/ipapadop-skills/skills/skill-provenance`. Re-run the command to update.

## Antigravity compatibility

The canonical skill uses the portable `SKILL.md` Agent Skills layout. Antigravity-specific automated installation is not documented here until Google publishes a stable repository or marketplace installation mechanism.
