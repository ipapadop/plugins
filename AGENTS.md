# AGENTS.md

## Layout

Each skill is in one plugin, in `plugins/<plugin>/skills/<skill>/`. Plugins group skills by topic:

- `ipapadop-documentation`: `code-documentation-style`, `simplified-technical-english`.
- `ipapadop-cpp`: `power-of-ten`.
- `ipapadop-skill-authoring`: `skill-provenance`.

Every directory in a `skills/` directory ships to all agents, so do not put unfinished skills there.

Claude Code and Codex both read the same two manifests:

- `.claude-plugin/marketplace.json`: the marketplace. It has no `version` field.
- `plugins/<plugin>/.claude-plugin/plugin.json`: one for each plugin. It has that plugin's `version` field.

Do not add `.codex-plugin/` or `.agents/plugins/marketplace.json`. Codex uses the `.claude-plugin/` files when they are absent.

## Versioning

When you add, remove, or change a skill, increase the `version` in the `plugin.json` of its plugin. Use semantic versioning: a new skill is a minor change, a fix to an existing skill is a patch.

Claude Code and Codex compare the `version` field to decide whether to update an installed plugin. If the version does not change, existing installs keep the old cached copy and do not get the new skills.

## Checks

Before you open a pull request, run:

```bash
python3 scripts/check-plugin.py --base origin/main
claude plugin validate . --strict
for manifest in plugins/*/.claude-plugin/plugin.json; do
  claude plugin validate "${manifest%/.claude-plugin/plugin.json}" --strict
done
```

`check-plugin.py` checks the skill metadata against the Codex limits (`description` at most 1024 characters, `short_description` 25 to 64 characters) and checks that changed skills come with a higher version of their plugin. CI runs the same checks.
