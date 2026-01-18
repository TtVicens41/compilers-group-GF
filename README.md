# Practice 1 – C Preprocessor (P1PP)

## 1. Project Goal

The goal of this project is to design and implement a **generic, extensible C preprocessor engine** that performs a subset of C preprocessing functionalities (comment elimination, directives, conditional inclusion, and substitution) over text files.

The solution is designed as an **engine**, independent from command-line flags, so that new preprocessing features or keywords can be added without modifying the core architecture.

---

## 2. Scope Definition

### 2.1 Supported Functionalities

* Comment elimination (`//`, `/* */`)
* `#define` (simple and extensible to macros)
* `#include "file"` (recursive, same-directory only)
* `#ifdef` / `#endif`
* Recursive preprocessing of included files
* Configurable execution via flags (`-c`, `-d`, `-all`, `-help`)

### 2.2 Explicitly Unsupported (but safely ignored)

* `#include <...>` (system headers)
* Full macro language
* Full C syntax validation
* Compiler-level semantic errors

Unsupported directives are copied unchanged to the output and never cause program termination.

---

## 3. High-Level Architecture Overview

The preprocessor is structured as a **pipeline-based engine** that processes input files line by line, applying a sequence of transformations depending on the configuration.

### Conceptual Flow

1. Read a line from the current input file
2. Update line counter
3. Remove comments (if enabled)
4. Detect directive
5. Dispatch directive handler OR process as normal text
6. Apply substitutions
7. Write to output (if enabled)

---

## 4. System Architecture Diagram

```
+------------------+
|      main.c      |
|------------------|
| Parse arguments  |
| Build config     |
| Create context   |
+--------+---------+
         |
         v
+--------------------------+
|   Preprocessor Engine    |
|    (preprocessor.c)     |
|--------------------------|
| Engine loop              |
| Pass orchestration       |
| Context coordination    |
+--------+-----------------+
         |
         v
+--------------------------+
|   Directive Dispatcher   |
|     (directives.c)      |
|--------------------------|
| Keyword detection        |
| Dispatch table lookup    |
+----+---------+-----------+
     |         |          |
     v         v          v
+---------+ +---------+ +---------+
| define  | | include | |  ifdef  |
| module  | | module  | | module  |
+---------+ +---------+ +---------+

(shared PreprocessorContext flows through all modules)
```

This diagram represents control flow and responsibility separation. No directive-specific logic exists in the engine.

---

## 5. Core Data Structure

### 5.1 PreprocessorContext

The **PreprocessorContext** is the central shared state of the system. All modules operate on this context to avoid global variables and to ensure extensibility.

Conceptually, it contains:

* Input and output streams
* Current file and line information
* Processing configuration (flags)
* Symbol table for definitions
* Conditional output state
* Error reporting context

This structure enables recursive includes, conditional compilation, and consistent error reporting.

---

## 6. Processing Pipeline (Engine Behavior)

The engine executes the following loop:

1. Fetch next line from input
2. Increment line counter
3. If comment removal enabled → strip comments
4. If directive detected:

   * Extract directive keyword
   * Dispatch to handler
   * Do not write directive to output
5. Else:

   * Apply substitutions
   * If output is enabled → write line to output

This pipeline is fixed and independent from command-line options.

---

## 7. Directive Dispatch Mechanism

Directive processing is implemented via **dispatch tables**, not conditional chains.

### Design Principles

* Each directive has exactly one handler
* The engine does not know directive semantics
* Adding a directive requires only adding a new table entry

### Conceptual Table

| Keyword | Handler Function |
| ------- | ---------------- |
| define  | handle_define    |
| include | handle_include   |
| ifdef   | handle_ifdef     |
| endif   | handle_endif     |

Unknown directives are ignored and copied unchanged.

---

## 8. Module Responsibilities

| Module         | Responsibility                    |
| -------------- | --------------------------------- |
| main.c         | CLI parsing, configuration setup  |
| preprocessor.c | Engine loop and orchestration     |
| context.h      | Shared data structure definitions |
| directives.c   | Directive detection and dispatch  |
| define.c       | Symbol table updates              |
| include.c      | Recursive include handling        |
| ifdef.c        | Conditional output control        |
| comments.c     | Comment elimination               |
| errors.c       | Centralized error reporting       |

Each module has a single responsibility and is independently testable.

---

## 9. Error Handling Strategy

Error handling is centralized and non-fatal by default.

For each error:

* File name is reported
* Line number is reported
* Relevant directive or identifier is included

The engine attempts to continue processing to provide maximum diagnostic information.

At least one error type is fully implemented (e.g., unmatched `#endif`). Others are documented as future extensions.

---

## 10. Command-Line Flags as Configuration

Flags are parsed only in `main.c` and translated into configuration fields inside the context.

The engine logic is **never modified** based on flags.

This guarantees architectural stability and extensibility.

---

## 11. Documentation Levels

This project includes documentation at three levels:

1. **User Manual** – usage, flags, error messages
2. **Conceptual Design Documentation** – this document and slides
3. **Code Documentation** – file headers, function headers, and conceptual comments

Documentation is maintained alongside design evolution.

---

## 12. Team Organization

Work is distributed by **module ownership**, not by flags.

Each module has:

* A responsible team member
* A corresponding design slide
* Clear interaction boundaries

This ensures parallel development without duplication.

---

## 13. Design Status

* Architecture: Defined and frozen (v1.0)
* Data structures: Defined
* Engine pipeline: Defined
* Directive dispatch: Defined
* Implementation: In progress

---

## 14. Conclusion

This design establishes a clean, extensible, and maintainable foundation for the C preprocessor. It satisfies all architectural, documentation, and extensibility requirements of the assignment and allows incremental, safe implementation of all requested functionalities.
