# skills

Collection of opinionated agent skills for Codex, Claude Code, and other tools that support the Agent Skills format.

## Available skills

- `documenting-skill-provenance`: creates evidence-based skill documentation with provenance, dependency, authorship, version, and release-history details.

## Install in Codex

### Install one skill

Ask Codex:

```text
Use $skill-installer to install plugins/ipapadop-skills/skills/documenting-skill-provenance from ipapadop/skills.
```

The skill becomes available on the next turn.

### Install the plugin

Register this repository as a marketplace and install the plugin:

```bash
codex plugin marketplace add ipapadop/skills
codex plugin add ipapadop-skills@ipapadop
```

Start a new Codex thread after installation so Codex discovers the plugin's skills.

## Install in Claude Code

Run these commands inside Claude Code:

```text
/plugin marketplace add ipapadop/skills
/plugin install ipapadop-skills@ipapadop
```

If the installation summary requests it, run `/reload-plugins`. The skill is available as `/ipapadop-skills:documenting-skill-provenance`.

## Install in Gemini CLI

Install the skill directly from its repository subdirectory:

```bash
gemini skills install https://github.com/ipapadop/skills.git \
  --path plugins/ipapadop-skills/skills/documenting-skill-provenance
```

## Antigravity compatibility

The canonical skill uses the portable `SKILL.md` Agent Skills layout. Antigravity-specific automated installation is not documented here until Google publishes a stable repository or marketplace installation mechanism.
