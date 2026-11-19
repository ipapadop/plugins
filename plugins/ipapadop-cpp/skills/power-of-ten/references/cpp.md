# Power of Ten in C++

The paper's rules target C. This file maps each rule to C++. The mappings are this skill's interpretation of the rule's intent: keep the call graph, loop bounds, memory use, and data flow provable by a tool. When you cite a mapping, say that it is an interpretation, not a rule from the paper.

Everything in [SKILL.md](../SKILL.md) still applies. This file adds only the C++ parts.

## Contents

1. Control flow
2. Loop bounds
3. Dynamic memory
4. Function length
5. Assertions
6. Scope
7. Return values and parameters
8. Preprocessor
9. Pointers and indirect calls
10. Warnings and analyzers

## 1. Control flow

- No `goto`, `setjmp`/`longjmp`, or runtime recursion, direct or indirect.
- Do not use exceptions for control flow. A `throw` is a non-local jump like `longjmp`: the path to the handler is not visible in the code. Compile with `-fno-exceptions` when the toolchain and libraries allow it. Return a status code, or use `std::expected` (C++23) or an equivalent result type. Mark it `[[nodiscard]]`.
- Remember that standard library calls can throw: `std::vector::at`, `std::stoi`, `new`, and any allocating container. Using them brings exceptions back.
- Template recursion and `constexpr` recursion that the compiler evaluates completely at compile time add no runtime cycle to the call graph. Allow them only in `consteval` functions or in templates, where no runtime call remains.
- Do not use coroutines. The control flow between suspension points is hidden.

## 2. Loop bounds

- A range-`for` over a `std::array` or a C array is bounded by its compile-time size.
- A range-`for` or a standard algorithm over a container whose size is not a compile-time constant is bounded only if that size is checked against a compile-time maximum. Assert it before the loop.
- Iterator loops over linked structures (`std::list`, intrusive lists, trees) need an explicit counter with a constant limit, as in C.
- `std::for_each`, `std::find_if`, and similar algorithms are loops. The same rule applies to them.

## 3. Dynamic memory

After initialization, do not allocate from the heap. In C++ allocation is often hidden. All of these allocate:

- `new`, `delete`, `std::make_unique`, `std::make_shared`.
- Growth of `std::vector`, `std::string`, `std::deque`, `std::map`, `std::set`, `std::unordered_map`, and `std::list` (`push_back`, `insert`, `emplace`, `resize`, `operator[]` on a map).
- `std::function` with a large capture, `std::any`, `std::regex`, iostreams, and thrown exceptions.

Use instead:

- `std::array`, C arrays, and fixed-capacity containers (a ring buffer or a static vector with a compile-time capacity that returns an error when full).
- `std::span` (C++20) or a pointer plus length for views into caller-owned storage.
- `std::string_view` for read-only text, and fixed `char` arrays for owned text.
- Static object pools created at startup.

A `std::vector` that is `reserve`d once in an init function and then never grows past its capacity complies in principle, but a tool can't easily prove it. Prefer a fixed-capacity type.

## 4. Function length

About 60 lines, as in C. A lambda body counts toward the function that contains it.

## 5. Assertions

- Use `static_assert` for everything known at compile time: sizes, capacities, type traits, enum counts. It is free and a tool can't defeat it.
- Runtime assertions follow the C rule: side-effect free, with an explicit recovery action. `assert()` aborts, so it does not satisfy the rule. The `c_assert` macro from SKILL.md works, or a helper function with a thin macro that supplies the expression text and location:

```cpp
[[nodiscard]] inline bool check(bool cond, const char* expr,
                                const char* file, int line) noexcept
{
    if (!cond) {
        report_assertion(expr, file, line);  // No-op on targets without output.
    }
    return cond;
}
#define CHECK(e) check(static_cast<bool>(e), #e, __FILE__, __LINE__)
```

## 6. Scope

- Declare variables at first use, in the smallest block. Use `if` and `switch` with an initializer (C++17) to limit scope.
- Prefer `const` and `constexpr`.
- Put file-local functions and objects in an unnamed namespace. Make class data members `private`.
- Do not use mutable global or `static` class data when a member or parameter works.

## 7. Return values and parameters

- Mark every function whose result must be checked `[[nodiscard]]`. Put `[[nodiscard]]` on status and result types so that every function that returns them is covered.
- To discard a result on purpose, cast to `(void)` and add a comment if the reason is not obvious.
- Check parameters as in C. Prefer types that make invalid values impossible: references instead of pointers that must not be null, `enum class` instead of `int`, `std::span` instead of pointer and length. A reference parameter needs no null check.

## 8. Preprocessor

- Use `constexpr` variables and functions, `enum class`, `inline` functions, and templates instead of macros.
- Use include guards or `#pragma once`.
- The C rules still apply: no `##`, no variadic macros, no recursive macros, and justify each conditional compilation directive. Prefer `if constexpr` to `#if` for choices the compiler can make.
- Do not use C-style variadic functions (`...`) in new code. They have no type checking. (This extends rule 8's ban on variadic macros to functions; it is an interpretation.) A variadic template is type checked and is acceptable when its expansion is simple.

## 9. Pointers and indirect calls

- One level of dereference, as in C. References count as pointers for this rule: `T**`, `T*&`, and chains such as `a->b->c` need a local copy and a check between steps.
- Smart pointers are pointers. Also, `std::unique_ptr` and `std::shared_ptr` usually imply heap allocation, which rule 3 forbids after init.
- Rule 9 forbids function pointers because they hide the call graph. In C++ these do the same and are also not allowed:
  - Virtual functions called through a base pointer or reference.
  - `std::function`, member function pointers, and stored lambdas called through a type-erased wrapper.
  - Tables of callbacks, such as `std::map<Id, std::function<...>>`.
- To select behavior, use a `switch` on an `enum class`, overloads, or templates (static polymorphism, CRTP), so that the call target is known at compile time. A lambda passed directly to a template parameter is resolved at compile time and is acceptable.
- If dynamic dispatch can't be avoided, list every possible target in a comment and justify it, as for a C function pointer.

## 10. Warnings and analyzers

Typical settings:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion \
    -Wshadow -Wcast-qual -Wold-style-cast -Wuseless-cast -Wnull-dereference \
    -Wnon-virtual-dtor -Woverloaded-virtual -Wformat=2 -Wvla \
    -fno-exceptions -fno-rtti -c file.cpp
clang-tidy file.cpp -checks='bugprone-*,cert-*,cppcoreguidelines-*,misc-*,performance-*,readability-*' -- -std=c++17
cppcheck --enable=all --inconclusive --language=c++ file.cpp
```

Drop `-fno-exceptions` or `-fno-rtti` only if the project requires them, and say so. As in C, rewrite code that confuses the compiler or the analyzer instead of suppressing the warning.
