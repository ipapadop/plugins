---
name: power-of-ten
description: Write, review, and refactor safety-critical C and C++ code with Gerard Holzmann's "Power of Ten" rules (NASA/JPL) - no recursion or goto, statically bounded loops, no heap allocation after initialization, short functions, recoverable assertions, checked return values, a restricted preprocessor, restricted pointers and no function pointers, and zero warnings at pedantic settings. Use whenever the user mentions safety-critical, mission-critical, flight, spacecraft, avionics, automotive ECU, medical-device, or similar high-assurance embedded code, the Power of Ten or JPL coding rules, or asks to make C/C++ code statically analyzable or to review it against these rules, even if the user does not name the rules. Not for ordinary application C/C++ where the user has not asked for these constraints.
---

# Power of Ten: safety-critical C and C++

Gerard J. Holzmann (NASA/JPL Laboratory for Reliable Software) proposed ten rules for safety-critical code. The rules are strict on purpose. Each one makes a property of the code provable by a tool: termination, bounded stack and memory, a known call graph, and a known data flow. Code that follows them can be checked mechanically, not only reviewed by hand. Apply them as written. When code can't comply, state the deviation and the reason in a comment at the site, so that a checker flags it and a reviewer can judge it.

The paper targets C. For C++, read [references/cpp.md](references/cpp.md) before you write or review code. It maps each rule to C++ features (exceptions, `new`, standard containers, virtual functions, `std::function`, templates). Those mappings are interpretations of this skill, not text from the paper; say so when you cite them.

## The rules

### 1. Simple control flow

Do not use `goto`, `setjmp`, `longjmp`, or direct or indirect recursion.

Without recursion the call graph is acyclic, so a tool can prove that execution is bounded and can compute the maximum stack depth. Early `return` for errors is allowed; the rule does not require a single exit point.

To remove recursion, use an explicit stack with a fixed capacity in static or caller-provided storage. When the stack is full, return an error. Do not grow it.

Look for indirect recursion too: `a()` calls `b()`, which calls `a()`. A function pointer or a callback can also hide a cycle (see rule 9).

### 2. Statically bounded loops

Every loop must have a fixed upper bound that a checking tool can prove trivially.

- A `for` loop over a compile-time constant or over a `size`/`count` that is checked against a compile-time maximum is bounded.
- A loop that walks a linked list, waits on hardware, retries, or reads until a sentinel is not bounded by itself. Add a counter with a constant limit. When the limit is reached, fail an assertion and return an error to the caller.
- A loop that must never end, such as a scheduler or the main control loop, takes the reverse rule: it must be provable that it cannot terminate. Write it as `for (;;)` or `while (1)` with no `break` and no `return`. Every loop inside it must be bounded.

```c
#define MAX_NODES 128U

int list_find(const struct node *head, int key, const struct node **out)
{
    const struct node *n = head;
    unsigned int i;

    if (!c_assert(out != NULL)) {
        return ERR_PARAM;
    }
    for (i = 0U; (i < MAX_NODES) && (n != NULL); i++) {
        if (n->key == key) {
            *out = n;
            return OK;
        }
        n = n->next;
    }
    if (!c_assert(n == NULL)) {   /* List is longer than MAX_NODES. */
        return ERR_BOUND;
    }
    return ERR_NOT_FOUND;
}
```

### 3. No dynamic memory allocation after initialization

Do not call `malloc`, `calloc`, `realloc`, `free`, or any other allocator after the initialization phase ends.

Allocators have unpredictable timing and can fail. Leaks, use-after-free, double free, and buffer overruns on allocated memory are a large class of defects. Use statically sized arrays, fixed-size pools allocated once at startup, and caller-provided buffers. Stack memory is acceptable: with no recursion, a tool can compute the stack bound. Do not use variable-length arrays or `alloca`; their size is not static, so the stack bound can't be proved.

If the code must allocate at startup, put every allocation in a clearly named init function, check each result, and never allocate again.

### 4. Short functions

No function is longer than one printed page with one line per statement and one line per declaration: about 60 lines.

Each function is one logical unit that a reader and a tool can verify alone. Split long functions into helpers with one purpose each. Do not meet the limit by putting several statements on one line.

### 5. Two assertions per function, with recovery

On average, write at least two assertions per function. Use them to check preconditions, parameters, return values, postconditions, and loop invariants: conditions that must never be false in a correct run.

- An assertion is a side-effect-free Boolean expression.
- When an assertion fails, the code takes an explicit recovery action, usually returning an error to the caller. The standard `assert()` does not satisfy this rule: it aborts the program, and `NDEBUG` removes it.
- An assertion that a tool can prove always true or always false (`assert(1)`, `x >= 0` for an unsigned `x`) does not count. Do not pad the count.

The paper's pattern is a macro that reports the failure and evaluates to `false`:

```c
#include <stdbool.h>

#define c_assert(e) ((e) ? true : \
    (tst_debugging("%s,%d: assertion '%s' failed\n", \
     __FILE__, __LINE__, #e), false))

if (!c_assert(p >= 0)) {
    return ERROR;
}
```

On a target with no output, `tst_debugging` becomes a no-op and `c_assert` becomes a plain Boolean test. Use `#e` stringizing here; the paper uses it in its own example. Use `static_assert` (C11) or `_Static_assert` for conditions known at compile time; they do not count as runtime checks but are good practice.

### 6. Smallest possible scope

Declare each data object at the smallest scope that works.

An object that is out of scope can't be read or corrupted, and fewer statements can have assigned a bad value. Prefer block-scope variables over function-scope ones, and `static` file-scope over global. Do not reuse one variable for two purposes.

### 7. Check return values and parameters

Every caller checks the return value of every non-void function, and every function checks its parameters.

- Propagate errors up the call chain.
- When the response to failure would be the same as to success (for example some `printf` calls), cast the result to `(void)` to show the choice is deliberate. In doubtful cases, add a comment that explains why the result does not matter.
- Check pointer parameters for `NULL`, index and length parameters against their limits, and enum parameters against their valid range.
- Standard library calls are included: `fclose`, `snprintf`, `write`, `strtol` (also check `errno` and the end pointer).

### 8. Limited preprocessor

Use the preprocessor only for `#include` and simple macro definitions.

- Do not use token pasting (`##`), variadic macros (`...`, `__VA_ARGS__`), or recursive macros.
- Every macro expands to a complete syntactic unit: a full expression in parentheses or a full statement. Prefer `static inline` functions and `enum` or `const` objects.
- Conditional compilation (`#if`, `#ifdef`) multiplies the versions to test: ten directives can make 2^10 variants. Apart from include guards, use it rarely, and add a comment at each use that justifies it.

The paper's own `c_assert` uses `#e` stringizing, `__FILE__`, and `__LINE__`; that is allowed.

### 9. Restricted pointers

- Use at most one level of dereference. Do not use `**p`, and do not dereference through a chain such as `p->q->r` in one expression; copy to a local and check it first. (Reading `p->q->r` as two levels is this skill's interpretation of the rule.)
- Do not hide a dereference in a macro or in a `typedef` (for example `typedef struct node *node_ptr;`).
- Do not use function pointers. They stop a tool from building the call graph and from proving there is no recursion. Use a `switch` on an `enum` to select behavior.

If a function pointer can't be avoided (for example a vector table that the hardware requires), justify it in a comment and document the complete set of possible targets, so that a reviewer can check the call graph by hand.

### 10. Zero warnings at the most pedantic setting

Compile all code from the first day with every warning enabled, and fix every warning. Check it with at least one, preferably more, static analyzer, with zero findings.

If the compiler or analyzer gives a false warning, rewrite the code so that it is trivially valid. Do not suppress the warning. A warning that looks invalid is often valid for a less obvious reason.

Typical settings:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion \
    -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wvla \
    -Wformat=2 -Wnull-dereference -fanalyzer -c file.c
clang-tidy file.c -- -std=c11
cppcheck --enable=all --inconclusive file.c
```

Use the analyzers that the project or the environment has. If you can run a compiler, compile your code with these flags and report the result. If you can't, say that the code was not compiled.

## Workflow

### Write new code

1. Find the compile-time limits first: maximum elements, maximum iterations, buffer sizes, retry counts. Make each one a named constant.
2. Allocate all storage statically or at init.
3. Write small functions. Each one checks its parameters, checks every call it makes, and has at least two meaningful assertions with recovery.
4. Use a status-code return (an `enum` of error codes) and pass outputs through pointer parameters.
5. Compile with the flags in rule 10 and fix every warning.
6. In the reply, list any deviation from a rule and its justification. Do not claim compliance that you did not check.

### Review existing code

Report each violation with its rule number, the line, what is wrong, and a concrete fix. Group findings by rule. Do not report code that complies; the exceptions in the rules are real (early returns, `(void)` casts, include guards, `#e` in an assertion macro, a provably infinite main loop). If a rule can't be checked from the code you have (for example function length in a truncated file), say so.

### Refactor to comply

Keep the observable behavior the same unless the user agrees to a change. When the original behavior is unbounded (for example recursion depth that depends on input), the bounded version must return a defined error when it reaches the limit; say what the new limit is and why that value. Test the old and new versions on the same inputs, including the inputs that reach each limit, if you can run code.

## Before you finish

Check the result against each rule:

1. No `goto`, `setjmp`/`longjmp`, or recursion, direct or indirect.
2. Every loop has a provable constant bound, or is a provably infinite top-level loop.
3. No allocation after init; no VLAs or `alloca`.
4. No function longer than about 60 lines.
5. At least two meaningful, side-effect-free assertions per function on average, each with a recovery action.
6. Each variable at its smallest scope.
7. Every non-void return value checked or cast to `(void)` with reason; every parameter checked.
8. No `##`, no variadic or recursive macros, every macro a complete unit, every `#if`/`#ifdef` justified.
9. No `**`, no hidden dereference, no function pointers.
10. Zero warnings with pedantic flags, or a statement that the code was not compiled.
