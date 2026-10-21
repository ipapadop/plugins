# AGENTS.md

## Layout

All skills are in one plugin, `ipapadop-skills`, in `plugins/ipapadop-skills/skills/<skill>/`. Every directory in `skills/` ships to all agents, so do not put unfinished skills there.

Claude Code and Codex both read the same two manifests:

- `.claude-plugin/marketplace.json`: the marketplace. It has no `version` field.
- `plugins/ipapadop-skills/.claude-plugin/plugin.json`: the plugin. It has the only `version` field.

Do not add `.codex-plugin/` or `.agents/plugins/marketplace.json`. Codex uses the `.claude-plugin/` files when they are absent.

## Versioning

When you add, remove, or change a skill, increase the `version` in `plugin.json`. Use semantic versioning: a new skill is a minor change, a fix to an existing skill is a patch.

Claude Code and Codex compare the `version` field to decide whether to update an installed plugin. If the version does not change, existing installs keep the old cached copy and do not get the new skills.

## Checks

Before you open a pull request, run:

```bash
python3 scripts/check-plugin.py --base origin/main
claude plugin validate . --strict
claude plugin validate plugins/ipapadop-skills --strict
```

`check-plugin.py` checks the skill metadata against the Codex limits (`description` at most 1024 characters, `short_description` 25 to 64 characters) and checks that changed skills come with a higher version. CI runs the same checks.
