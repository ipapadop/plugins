# Code Documentation Style

## What this skill does

The skill makes an agent write code documentation in clear, compact technical language. It applies the Google developer documentation style guide to docstrings, API reference comments, inline code comments, TODOs, deprecation notices, and READMEs that ship with code. It removes filler, comments that restate the code, and comments that describe the edit history.

## How it works

### Trigger conditions

The agent loads the skill when it writes, adds, or edits docstrings, API reference comments (Javadoc, JSDoc, Doxygen, rustdoc, Go doc, Python docstrings), inline code comments, TODOs, deprecation notices, or a README or usage document that ships with code. The user does not have to mention style. The skill does not apply to tutorials, blog posts, marketing copy, or chat replies.

### Workflow

1. The agent reads nearby code and follows the project's documentation conventions first, then the language's conventions. The skill rules apply where both are silent.
2. The agent documents public members within the requested scope or changed API. It leaves unrelated members alone and adds an inline comment only when the code cannot explain itself.
3. The agent writes each comment with the skill's rules: a verb-first summary sentence, fixed wording for parameters, return values, exceptions, and deprecations, the language rules, and the rules for code in text and READMEs.
4. Before it finishes, the agent checks each comment against a five-item list.

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
| Google developer documentation style guide: Highlights | https://developers.google.com/style/highlights | Not applicable | Last updated 2025-04-02 | Not applicable |
| Google developer documentation style guide: API reference code comments | https://developers.google.com/style/api-reference-comments | Not applicable | Last updated 2026-07-01 | Not applicable |
| Google developer documentation style guide: Code in text | https://developers.google.com/style/code-in-text | Not applicable | Last updated 2026-01-06 | Not applicable |
| Google developer documentation style guide: Voice and tone | https://developers.google.com/style/tone | Not applicable | Last updated 2026-05-27 | Not applicable |
| Google developer documentation style guide: Present tense | https://developers.google.com/style/tense | Not applicable | Last updated 2024-10-15 | Not applicable |
| Google developer documentation style guide: Contractions | https://developers.google.com/style/contractions | Not applicable | Last updated 2025-02-21 | Not applicable |
| Google developer documentation style guide: Code samples | https://developers.google.com/style/code-samples | Not applicable | Last updated 2025-10-10 | Not applicable |
| Google developer documentation style guide: Placeholders | https://developers.google.com/style/placeholders | Not applicable | Last updated 2026-08-13 | Not applicable |
| Google developer documentation style guide: Command-line syntax | https://developers.google.com/style/code-syntax | Not applicable | Last updated 2025-10-10 | Not applicable |
| Google Python Style Guide, section 3.8 (Comments and Docstrings) | https://google.github.io/styleguide/pyguide.html | `fc981500047ef2cc2e2509b00dcd23c9badaf8e4` (`gh-pages` branch of https://github.com/google/styleguide) | Not documented | Not applicable |

The skill paraphrases the sources. It does not copy their text. The Python style guide supplies the rule "never describe the code" and the TODO format. The other rules come from the developer documentation style guide.

## Version

1.0.1

## Authors

- Yiannis Papadopoulos

## Release notes

Newest releases appear first.

### 1.0.1

- Date: 2026-10-02
- Changes: Limits documentation edits to the requested scope or changed API. Adds evaluations for scope, project conventions, and factual accuracy.

### 1.0.0

- Date: 2026-10-02
- Changes: Initial release. Applies the Google developer documentation style guide to docstrings, API reference comments, inline comments, TODOs, deprecation notices, and READMEs that ship with code. Project and language conventions take precedence.
