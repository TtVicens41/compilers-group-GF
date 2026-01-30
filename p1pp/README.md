# Practice 1 – C Preprocessor (P1PP)

## 1. Project Goal

This project implements a **simple C preprocessor** that removes comments and handles basic preprocessor directives from C source files.

The design follows a modular, extensible architecture where the engine orchestrates processing and delegates specific tasks to specialized modules.

---

## 2. Features

### 2.1 Currently Implemented

* **Comment removal**: Both single-line (`//`) and multi-line (`/* */`) comments
* **Directive detection**: Recognizes lines starting with `#`
* **Dispatch table**: Extensible handler registration for directives
* **Configurable flags**: `-c`, `-d`, `-all`, `-help`

### 2.2 Planned (Stubs Ready)

* `#define` – macro definitions
* `#include "file"` – local file inclusion
* `#ifdef` / `#endif` – conditional compilation

### 2.3 Out of Scope

* `#include <...>` (system headers)
* Complex macro expansion
* Full C syntax validation

Unsupported directives are safely ignored and passed through to output.

---

## 3. Project Structure

```
src/
├── main.c                    # Entry point, argument parsing
├── context.h                 # Shared PreprocessorContext structure
├── preprocessor/
│   ├── preprocessor.h        # Engine interface
│   └── preprocessor.c        # Main processing loop
├── comments/
│   ├── comments.h            # Comment removal interface
│   └── comments.c            # Comment stripping logic
└── directives/
    ├── directives.h          # Directive dispatcher interface
    └── directives.c          # Directive detection and dispatch table
```

---

## 4. Architecture Diagram

```
┌──────────────────────────────────────────────────────────┐
│                        main.c                            │
│  • Parse command-line arguments                          │
│  • Initialize PreprocessorContext                        │
│  • Open input/output files                               │
│  • Call run_preprocessor()                               │
└────────────────────────┬─────────────────────────────────┘
                         │
                         ▼
┌──────────────────────────────────────────────────────────┐
│              preprocessor.c (Engine)                     │
│  • Read file line by line                                │
│  • Call remove_comments() if enabled                     │
│  • Call process_directive() if enabled                   │
│  • Write processed lines to output                       │
└────────────┬─────────────────────────┬───────────────────┘
             │                         │
             ▼                         ▼
┌────────────────────────┐   ┌─────────────────────────────┐
│    comments.c          │   │      directives.c           │
│  • Strip // comments   │   │  • Detect # directives      │ 
│  • Strip /* */ blocks  │   │  • Lookup in dispatch table │
│  • Track block state   │   │  • Call appropriate handler │
└────────────────────────┘   └──────────────┬──────────────┘
                                            │
                         ┌──────────────────┼──────────────────┐
                         ▼                  ▼                  ▼
                   ┌──────────┐      ┌──────────┐       ┌──────────┐
                   │ #define  │      │ #include │       │ #ifdef   │
                   │ (stub)   │      │ (stub)   │       │ (stub)   │
                   └──────────┘      └──────────┘       └──────────┘

         All modules share the PreprocessorContext structure
```

---

## 5. Core Data Structure

### PreprocessorContext (context.h)

The central structure that holds all shared state:

```c
typedef struct {
    FILE *input;              // Input file stream
    FILE *output;             // Output file stream
    const char *input_filename;
    const char *output_filename;
    int current_line;         // Current line number
    bool remove_comments;     // Flag: strip comments?
    bool process_directives;  // Flag: handle directives?
    bool output_enabled;      // Flag: write to output?
    void *symbol_table;       // For future #define storage
    bool in_block_comment;    // Tracks /* */ state
} PreprocessorContext;
```

This structure is passed to all modules, avoiding global variables.

---

## 6. Processing Pipeline

The engine (`preprocessor.c`) processes each line through these steps:

```
┌─────────────────────────────────────────────────┐
│  1. Read line from input                        │
│  2. Increment line counter                      │
│  3. If remove_comments enabled:                 │
│     └── Call remove_comments(line)              │
│  4. If process_directives enabled:              │
│     └── Call process_directive(line)            │
│         └── If directive found: skip output     │
│  5. If output_enabled:                          │
│     └── Write line to output file               │
└─────────────────────────────────────────────────┘
```

---

## 7. Directive Dispatch System

Directives are handled via a **dispatch table**, making it easy to add new handlers:

```c
static DirectiveEntry directive_table[] = {
    {"define",  handle_define},
    {"include", handle_include},
    {"ifdef",   handle_ifdef},
    {"endif",   handle_endif},
    {NULL, NULL}  // Sentinel
};
```

**Adding a new directive** only requires:
1. Write the handler function
2. Add an entry to the table

No changes to the engine are needed.

---

## 8. Module Responsibilities

| File              | Responsibility                              |
|-------------------|---------------------------------------------|
| `main.c`          | CLI handling, context setup, file I/O       |
| `context.h`       | Shared data structure definition            |
| `preprocessor.c`  | Main loop, orchestrates processing          |
| `comments.c`      | Removes // and /* */ comments               |
| `directives.c`    | Detects and dispatches # directives         |

---

## 9. Usage

```bash
# Build the project
cd src && make

# Run preprocessor
./preprocessor input.c

# Output is written to output_pp.c
```

### Command-Line Flags (Planned)

| Flag    | Description                              |
|---------|------------------------------------------|
| `-c`    | Remove comments only (default)           |
| `-d`    | Process directives only                  |
| `-all`  | Remove comments and process directives   |
| `-help` | Show usage information                   |

---

## 10. Error Handling

The preprocessor follows a **continue-on-error** approach:
* Errors are reported with file name and line number
* Processing continues to provide maximum diagnostic info
* Unsupported directives are passed through unchanged

---

## 11. Current Status

| Component           | Status        |
|---------------------|---------------|
| Architecture        | ✅ Complete   |
| Context structure   | ✅ Complete   |
| Engine loop         | ✅ Complete   |
| Comment removal     | ✅ Complete   |
| Directive detection | ✅ Complete   |
| Dispatch table      | ✅ Complete   |
| #define handler     | 🔲 Stub       |
| #include handler    | 🔲 Stub       |
| #ifdef/#endif       | 🔲 Stub       |
| CLI argument parser | 🔲 Pending    |

---

## 12. Next Steps

1. Implement CLI argument parsing with flag support
2. Implement `#define` macro storage and substitution
3. Implement `#include` file expansion
4. Implement `#ifdef`/`#endif` conditional compilation
5. Add comprehensive error reporting
