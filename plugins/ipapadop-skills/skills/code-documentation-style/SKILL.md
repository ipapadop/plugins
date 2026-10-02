---
name: code-documentation-style
description: Write code documentation in clear, compact technical language based on the Google developer documentation style guide. Use whenever you write, add, or edit docstrings, API reference comments (Javadoc, JSDoc, Doxygen, rustdoc, Go doc, Python docstrings), inline code comments, TODOs, deprecation notices, or a README or usage doc that ships with code, even if the user does not mention style. Removes filler, restated code, and edit narration. Follow a project's existing doc conventions first. Not for tutorials, blog posts, marketing copy, or chat replies.
---

# Code documentation style

Document code so that a developer can use or change it after one read. Every sentence must tell the reader something the code, the signature, or the name does not already tell them. This skill applies the Google developer documentation style guide to code documentation.

Filler costs the reader time and hides the facts. Stale or restated comments are worse than none, because readers trust them.

## Precedence

1. The project's existing conventions: docstring format (Google, NumPy, Sphinx, Javadoc), tag set, line length, and voice. Read nearby code before you write.
2. The language's own documentation convention. For example, Go doc comments start with the identifier name ("Fetch returns ..."), and Rust uses `# Examples` and `# Errors` sections.
3. The rules in this skill, for everything the first two leave open.

For code documentation, this skill takes precedence over general prose-style skills such as `simplified-technical-english`. For example, use contractions here even if that skill is also loaded.

## What to document

- Document every public class, interface, struct, enum, constant, field, function, and method. Describe each parameter, the return value, and each exception or error the caller must handle.
- Write a comment inside a function only when the code cannot say it: why a choice was made, a non-obvious constraint, a workaround and its cause, a units or ownership rule. Assume the reader knows the language better than you do.
- Do not describe what the next line does (`# Increment the counter`). Do not restate the name or the type signature.
- Do not narrate your edit. Comments such as "Updated to use the new client", "Fixed bug where...", or "Now handles None" describe history, not code. History belongs in the commit message.
- Do not leave commented-out code, decorative banners, or section dividers.
- Format a TODO as `TODO: <issue link or owner> - <action>`, for example `# TODO: crbug.com/192795 - Remove after the v2 migration.`

## Summary sentence

The first sentence of a doc comment appears alone in indexes, tooltips, and generated summaries. Make it complete, specific, and short.

- Start a function or method summary with a third-person, present-tense verb:
  - "Adds ...", "Creates ...", "Parses ...", for operations.
  - "Gets the ..." or "Returns the ..." for getters.
  - "Checks whether ..." for boolean methods.
  - "Sets ..." or "Registers ..." for methods without a return value.
  - "Called when ..." or "Called by ..." for callbacks.
- Start a class or type summary with a noun phrase that states what it is or holds: "A thread-safe cache of parsed templates."
- Do not repeat the name, and do not open with "This method", "This class will", or "A function that". (Go is the exception: its convention starts with the name.)
- Make sibling summaries distinct. "Gets the timeout." and "Gets the timeout for idle connections." must not both read "Gets the timeout."
- Put usage, invariants, thread safety, performance, and examples after the summary, in a separate paragraph.

## Parameters, returns, and errors

- Start each description with a capital letter and end it with a period.
- Non-boolean parameter: start with "The" or "A". `user_id: The ID of the user to delete.`
- Boolean parameter that triggers an action: "If `true`, ... If `false`, ..." Use the language's spelling of the literal (`True` in Python).
- Boolean parameter that describes a state: "`true` if ...; `false` otherwise."
- Parameter with a default: state "Default: `30`." only when the signature doesn't show the default, or explain what a sentinel default means ("If `None`, waits with no limit.").
- Give units, valid ranges, and whether `None` or `null` is accepted, when the type does not say so.
- Return value: start with "The" or "A". Boolean returns use "`true` if ...; `false` otherwise." Keep it short; put long explanations in the main description.
- Exceptions: "If <condition>." when the tag already says "Raises" or "Throws"; otherwise "Thrown when <condition>."
- If one sentence fully describes a trivial member, such as a getter or a deleted copy constructor, write only that sentence or nothing. Don't repeat the summary in a `@return` tag.

## Deprecation

Put the replacement in the first sentence, then the version and migration steps.

```java
/**
 * @deprecated Use {@link #fetchAll(Query)} instead. Removed in 3.0.
 *     Pass {@code Query.all()} to get the previous behavior.
 */
```

## Language

- Use present tense for behavior: "Returns the cached value." Use "will" only for an event that happens later, such as an asynchronous callback. Do not use "would" for normal behavior.
- Use active voice and name the actor: "The scheduler retries the task", not "The task is retried".
- In READMEs and usage docs, address the reader as "you". Do not use "we" or "let's".
- Cut filler: "please", "please note", "note that", "simply", "just", "easy", "easily", "quickly", "basically", "obviously", "at this time", "in order to" (write "to"), "it is important to". Do not use exclamation marks.
- Do not pre-announce features or describe plans ("will support streaming soon").
- Use common contractions, especially for negation: "doesn't", "isn't", "can't". A reader who scans can miss "not"; "don't" is harder to misread.
- Write "for example" and "that is", not "e.g." and "i.e."
- Put a condition before the instruction: "To retry, call `reset()` first."
- Define a term or abbreviation at first use. Use one name for one thing.
- Avoid idioms, metaphors, humor, and slang. Many readers use a translator.

## Code in text

- Use the documentation tool's code markup and match the file: `{@code}` in Javadoc, `@c`, `@p`, or backticks in Doxygen, double backticks in reStructuredText, and single backticks in Markdown.
- Put code entities in code font: names of classes, methods, parameters, fields, files, paths, commands, flags, environment variables, data types, literal values, and HTTP methods and status codes (`404 Not Found`).
- Do not inflect a code entity or use it as a verb. Write "the `Config` object's fields", not "`Config`'s fields". Write "send a `POST` request", not "`POST` the data".
- Write a method name without the class name unless the context is ambiguous.

## READMEs and usage docs

- Use sentence case for headings: "Configure the client", not "Configure The Client".
- Use numbered lists for steps in order and bulleted lists for other items. Use the serial comma.
- Introduce each code block with a sentence. End it with a colon if the block follows directly.
- Make commands copy-paste ready. Do not include `[optional]`, `{a|b}`, `...`, or a `$` prompt in a command the reader runs. Put command output in a separate block.
- Name placeholders in `UPPER_SNAKE_CASE`, then explain them: "Replace `PROJECT_ID` with your project ID." For several placeholders, write "Replace the following:" and a list.
- Show command output only when the reader must check or copy a value. Introduce it with "The output is similar to the following:".
- Use descriptive link text: "see the [configuration reference](...)", not "click [here](...)".
- Write dates unambiguously: "2026-10-02" or "October 2, 2026".

## Example

Verbose:

```python
def load_config(path, strict=False):
    """This function is used in order to load the configuration file.

    Basically, it simply reads the file at the given path and parses it.
    Please note that if strict is set to True, then unknown keys will cause
    an error to be raised. We updated this to support YAML as well!

    Args:
        path: path
        strict: whether strict mode is on

    Returns:
        the config
    """
```

Revised:

```python
def load_config(path, strict=False):
    """Loads a configuration from a JSON or YAML file.

    Args:
        path: The path to the configuration file. The extension selects the
            parser.
        strict: If `True`, raises an error for unknown keys. If `False`,
            ignores them.

    Returns:
        A `Config` object with defaults applied for missing keys.

    Raises:
        ConfigError: If the file can't be parsed, or if `strict` is `True`
            and the file has an unknown key.
    """
```

## Before you finish

Check each comment once:

- The summary starts with a verb (functions) or a noun phrase (types) and doesn't repeat the name, unless the language convention requires the name, as Go does.
- No comment restates the code, narrates an edit, or contains filler from the list above.
- Each parameter, return value, and error the caller handles is described, with units and defaults.
- Code entities are in code font.
- The documentation matches the code as it is now. Shorter is not better if it removes a fact the caller needs.
