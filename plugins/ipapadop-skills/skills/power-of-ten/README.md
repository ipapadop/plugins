# Power of Ten

## What this skill does

The skill makes an agent write, review, and refactor safety-critical C and C++ code with Gerard J. Holzmann's ten rules from "The Power of Ten – Rules for Developing Safety Critical Code" (NASA/JPL Laboratory for Reliable Software). The rules forbid recursion, `goto`, and `setjmp`/`longjmp`; require statically bounded loops; forbid heap allocation after initialization; limit functions to about 60 lines; require at least two recoverable assertions per function on average; require the smallest possible data scope; require checked return values and parameters; restrict the preprocessor; restrict pointers and forbid function pointers; and require zero warnings at pedantic compiler settings and from static analyzers.

## How it works

### Trigger conditions

The agent loads the skill when the user works on safety-critical, mission-critical, flight, spacecraft, avionics, automotive ECU, medical-device, or similar high-assurance embedded C or C++ code, mentions the Power of Ten or JPL coding rules, or asks to make C or C++ code statically analyzable or to review it against these rules. The skill does not apply to ordinary application C or C++ where the user has not asked for these constraints.

### Workflow

1. For C++, the agent reads `references/cpp.md`, which maps each rule to C++ features such as exceptions, hidden allocation in standard containers, virtual functions, `std::function`, `[[nodiscard]]`, and templates. These mappings are the skill's interpretation of the rules, not text from the paper, and the agent says so when it cites them.
2. To write new code, the agent defines compile-time limits first, allocates storage statically or at initialization, writes small functions that check parameters and return values and use recoverable assertions, returns status codes, compiles with pedantic warnings, and lists any deviation from a rule.
3. To review code, the agent reports each violation by rule with its line and a fix, and does not report the constructs the rules allow (early returns, `(void)` casts, include guards, `#e` in an assertion macro, a provably infinite top-level loop).
4. To refactor code, the agent keeps the observable behavior, adds a defined error at each new limit, states the limit, and tests the old and new versions on the same inputs.
5. Before it finishes, the agent checks the result against a ten-item list, one item per rule.

## Dependencies

### Scripts and runtimes

None.

### Packages

None.

### Shell and operating-system commands

Optional, used when available: `gcc` or `g++`, `clang-tidy`, and `cppcheck`. The skill tells the agent to compile with pedantic flags and run static analyzers when it can, and to say that the code was not compiled when it can't.

### External tools and services

None.

### Other skills

None.

## Sources

| Source | Link or local path | Commit/tag | Version/release | Local-source description |
| --- | --- | --- | --- | --- |
| Gerard J. Holzmann, "The Power of Ten – Rules for Developing Safety Critical Code" | https://spinroot.com/gerard/pdf/P10.pdf | Not applicable | Not documented in the document. PDF metadata creation date and server last-modified date: 2026-02-15. SHA-256 of the file used: `2217f41df961b9ed25cc072700b6332d6193799ed09d83be48bf791d05025f54` | Not applicable |

The skill paraphrases the paper. It reproduces the paper's `c_assert` macro pattern. The C++ mappings in `references/cpp.md` and the example compiler and analyzer flag sets are the skill's own interpretation and have no external source.

## Version

0.1.0

## Authors

- Yiannis Papadopoulos

## Release notes

Newest releases appear first.

### 0.1.0

- Date: 2026-10-21
- Changes: Initial release. Applies Holzmann's ten rules to writing, reviewing, and refactoring safety-critical C, with a C++ interpretation of each rule.
