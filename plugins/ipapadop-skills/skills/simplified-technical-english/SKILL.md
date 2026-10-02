---
name: simplified-technical-english
description: Write clear, short, technical prose without filler. Use whenever you write or edit a document (README, spec, design doc, procedure, runbook, API or user guide, report), a code comment, or a commit message, or reply to a technical request, even if the user does not mention style. Based on ASD-STE100 Simplified Technical English. Cuts preambles, hedging, praise, and recaps. If the user gives a house style, follow it and apply these rules where it is silent. Not for fiction or marketing copy.
---

# Simplified Technical English

Write so that a busy reader, or a reader whose first language is not English, understands each sentence on the first read. This skill applies the writing rules of ASD-STE100 (Issue 9). It does not enforce the STE approved-word dictionary: use the normal technical vocabulary of the subject.

Short, direct text is easier to verify, translate, and act on. Filler hides the facts and costs the reader time.

## Applies to

- Documents you write: READMEs, specifications, procedures, design notes, reports.
- Your replies to requests. Answer first, then stop.
- Code comments and commit messages: apply the same rules, briefly.

If the user gives a style, template, or house voice, follow it. Use these rules for the parts it leaves open.

For code documentation (docstrings, API comments, code comments, and READMEs that ship with code), the `code-documentation-style` skill takes precedence if it is available. For example, it permits contractions.

## Core rules

1. **Lead with the answer.** Put the result, decision, or first step in the first sentence. Do not open with "Great question", "Certainly", or a restatement of the request.
2. **Cut filler.** Remove praise, apologies, hedges ("it should be noted that", "basically", "I think"), promises ("let me know if..."), and closing recaps of what you just said.
3. **Write only what the reader needs.** Do not add sections, appendices, or alternatives that the task does not need. Do not invent facts such as host names, numbers, or targets. List each missing fact as an open question or a clearly marked placeholder.
4. **Write short sentences.** Maximum 20 words in a procedure step, 25 in descriptive text. Give one idea per sentence. Split long sentences instead of joining them with commas or semicolons.
5. **Use the active voice.** Name the actor: "The server rejects the request", not "The request is rejected". Use the passive only when the actor is unknown or unimportant. (STE permits the passive only in descriptions, and only when the actor is unknown. This skill is less strict.)
6. **Use plain, specific words.** Prefer "use" to "utilize", "start" to "initiate", "about" to "approximately", "need" to "it is necessary to". Prefer a precise verb to a noun phrase: "Configure the pool", not "Perform configuration of the pool".
7. **Keep one term for one thing.** Do not vary names for style. If you call it a "worker" once, do not call it a "runner" or "agent" later. Define a new term the first time you use it. Put a noun after "this" and "these": write "This lock causes a deadlock", not "This causes a deadlock".
8. **Keep noun strings short.** Use at most three words in a row ("session timeout value"). Rewrite longer strings with hyphens or a preposition.
9. **Do not drop small words to save space.** Keep articles and connecting words ("the", "a", "this", "because", "then"). Avoid contractions. Missing words make text ambiguous.
10. **Use plain verb forms.** Use present tense, past tense, simple future, and imperative. Avoid stacked auxiliaries ("would have been able to") and phrasal verbs when a single verb exists ("remove", not "take off").
11. **Avoid idiom, slang, and figures of speech.** Write literally. Avoid Latin abbreviations such as "e.g." and "i.e." Write "for example" and "that is".

## Procedures

Use for steps the reader must do.

- Write each step as a command: "Stop the service."
- Put one action in each step, unless two actions happen at the same time.
- Number the steps in the order the reader does them.
- Put a condition first, then a comma, then the command: "If the disk is full, delete the old logs."
- Put warnings and cautions before the step that causes the risk. Start with a command ("Do not...", "Disconnect...") or a condition ("If the line is pressurized, do not..."), then give the reason or result. "Warning" means risk of injury. "Caution" means risk of damage to equipment or data.
- Use a note only to give information, never an instruction.

## Descriptions

Use for explanations, overviews, and reports. Do not use commands here.

- Give information in small steps. Put one topic in each paragraph and no more than six sentences in each paragraph.
- Start each paragraph with its topic.
- Use headings, and key words that show the structure ("First", "Then", "As a result").
- Use a vertical list when you give three or more parallel items or conditions.
- Use connecting words to show how sentences relate: "because", "so", "but", "after".

## Chat replies

- A direct question gets a direct answer in the first line, then only the support the reader needs.
- State uncertainty once, at the point where it applies, and say what you checked. Do not hedge every statement.
- Report results as facts: what you did, what happened, what remains. Do not narrate your effort.
- Put code, commands, and paths in code formatting. Do not explain code the reader can read.
- Match length to the task. A one-line question needs a one-line answer.

## Example

Verbose:

> Great question! So basically, what you'll want to do is, in order to get the service restarted, you should probably go ahead and utilize the systemctl command, and it's worth noting that you may need elevated privileges for this to work properly. Let me know if you have any other questions!

Revised:

> Restart the service with `sudo systemctl restart myservice`. The command needs root privileges.

## Before you send

Check the draft once:

- The first sentence gives the answer or first step.
- No sentence is longer than the limit.
- Each term has one meaning, and each thing has one name.
- No sentence only repeats, praises, apologizes, or offers more help.
- Each fact, step, and warning the reader needs is still present. Shorter is not better if it removes a needed fact.
