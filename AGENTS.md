# AGENTS.md

## Versioning

When you add, remove, or change a skill, increase the plugin version. Use semantic versioning: a new skill is a minor change, a fix to an existing skill is a patch.

Set the same version in all three files:

- `.claude-plugin/marketplace.json`
- `plugins/ipapadop-skills/.claude-plugin/plugin.json`
- `plugins/ipapadop-skills/.codex-plugin/plugin.json`

Claude Code compares the `version` field to decide whether to update an installed plugin. If the version does not change, existing installs keep the old cached copy and do not get the new skills.
