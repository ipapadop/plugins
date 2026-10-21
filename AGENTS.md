# AGENTS.md

## Versioning

Each skill is a separate plugin in `plugins/<skill>/`. Each plugin has its own version.

When you add, remove, or change a skill, increase the version of its plugin only. Use semantic versioning: a fix to an existing skill is a patch, and a new skill is a new plugin that starts at `0.1.0`.

Set the same version in all three files:

- `.claude-plugin/marketplace.json` (the entry for the plugin)
- `plugins/<skill>/.claude-plugin/plugin.json`
- `plugins/<skill>/.codex-plugin/plugin.json`

When you add a plugin, also add it to `.agents/plugins/marketplace.json`.

Claude Code compares the `version` field to decide whether to update an installed plugin. If the version does not change, existing installs keep the old cached copy and do not get the new skills.
