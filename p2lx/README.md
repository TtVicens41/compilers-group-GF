# Practice 2: Lexical Analysis (P2LX)

Authors of this file:<br>
- Joan Vicente Martín<br>
- Pau Puig Guillén<br>
- ...

Reviewed by:<br>
- Marc Bosch Manzano

Creation:<br>
- 2026-02-16

## Overview

`p2lx` implements a character-by-character lexical scanner for the P2 assignment.
It reads a `.c` input file, generates a token stream in memory, and writes a token
file with extension `<input>.cscn`.

The scanner supports:
- Keywords: `if`, `else`, `while`, `return`, `int`, `char`, `void`
- Operators: `=`, `>`, `+`, `*`
- Special characters: `(` `)` `;` `{` `}` `[` `]` `,`
- Numbers: integer sequences `[0-9]+`
- Identifiers: `[A-Za-z][A-Za-z0-9]*`
- Literals: text inside double quotes `"..."` (including quotes)
- Non-recognized lexemes with informative errors

## Build

```bash
./build_local.sh
```

You can pass compile-time configuration flags through `EXTRA_CFLAGS`:

```bash
EXTRA_CFLAGS='-DOUTFORMAT=OUTFORMAT_RELEASE -DDEBUG=DEBUG_OFF' ./build_local.sh
```

## Usage

```bash
./lexer <input_file.c>
./lexer -help
```

Output file is generated as:

```text
<input_file.c>scn
```

Example:
- Input: `example_app.c`
- Output: `example_app.cscn`

## Output Formats (`OUTFORMAT`)

Configured at compile time in `src/lexer_config.h` or via `EXTRA_CFLAGS`.

- `OUTFORMAT_RELEASE`: one token line per non-empty input line; no empty lines.
- `OUTFORMAT_DEBUG`: same line mapping, with input line number prefix and an
  empty separator line after each token line.

Token text format:

```text
<lexeme, CAT_CATEGORY>
```

## Message Routing (`DEBUG`)

- `DEBUG_ON`: messages/errors are written to the scanner output file.
- `DEBUG_OFF`: messages/errors are written to stdout.

## Counter Configuration

Counters are enabled only when:
- `COUNTCONFIG == COUNT_ENABLED`
- `OUTFORMAT == OUTFORMAT_DEBUG`

Available counters:
- `COUNTCOMP`: comparisons
- `COUNTIO`: I/O operations (counted by character amount)
- `COUNTGEN`: generic operations

Output routing:
- `COUNTOUT == OUT`: count messages go to scanner output file.
- `COUNTOUT == DBGCOUNT`: count messages go to `<input>.cdbgcnt`.

Each count line includes:
- input line number
- function name
- increment amount
- partial per function
- total global counters

## Architecture

- `src/automata/`: DFA/NFA parser and lexeme classification
- `src/lexer.c`: scanner core, line processing, output generation
- `src/token.c`: token categories and dynamic token list
- `src/counter.c`: operation counting and reporting
- `src/utils/error*.c`: error IDs, templates and reporting

## Parser Hook (P3 preparation)

`run_pipeline_with_optional_parser(...)` provides a hook point for chaining the
future parser phase without redesigning the scanner pipeline.

## Acceptance Test Script

```bash
./tests/run_acceptance.sh
```

This script validates:
- token categories
- release/debug output behavior
- non-recognized grouping
- count file generation and contents

## Notes

- Input is expected to be preprocessed (no `#` directives and no comments), as
  requested by the assignment.
- The scanner continues after errors and emits `CAT_NONRECOGNIZED` tokens.
