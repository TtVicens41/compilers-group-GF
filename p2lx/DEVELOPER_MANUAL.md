# P2LX Developer Manual

Authors of this file:<br>
- Joan Vicente Martín<br>
- Pau Puig Guillén<br>
- ...

Reviewed by:<br>
- Marc Bosch Manzano

Creation:<br>
- 2026-02-16

## 1. Scope

This document describes implementation decisions for the P2 lexical scanner,
including automata approach, error model, counters, and P3 integration hook.

## 2. Core Design

The scanner processes input **character by character** and emits tokens in order.

Pipeline:
1. Read one input line.
2. Split into lexemes by scanner rules.
3. Classify each lexeme by automata (and literal state machine).
4. Append token to in-memory `TokenList`.
5. Render line to output format (`RELEASE` or `DEBUG`).

## 3. Automata Strategy

Implementation uses a union-NFA as a list of DFA automata loaded from
`resources/automata.txt`.

`classify_lexeme_nfa(...)` iterates all DFAs in priority order and returns first
accepted category. This allows keyword-over-identifier priority by ordering.

### 3.1 Regex / lexical models

- Keyword DFAs:
  - `if|int` (shared prefix automaton)
  - `else`
  - `while`
  - `return`
  - `void`
  - `char`
- Operator DFA: `[=>+*]`
- Special char DFA: `[;,(){}\[\]]`
- Number DFA: `[0-9]+`
- Identifier DFA: `[A-Za-z][A-Za-z0-9]*`
- Literal: dedicated state machine in scanner: `"..."` (unterminated handled as error)

### 3.2 Why literal is in scanner core

Literal content can include broad alphabet characters and is naturally handled as
line-local scanning state. This still follows automata principles (state
transitions in code), while keeping `automata.txt` manageable and robust.

## 4. Error Model

Errors use:
- unique `ErrorCode` numeric IDs
- compiler step ID (`STEP_LEXER`)
- contextual payload: line, column, lexeme, extra details

Current lexical errors:
- `2001 ERR_NON_RECOGNIZED`
- `2002 ERR_UNTERMINATED_LITERAL`
- `2003 ERR_FILE_INPUT`
- `2004 ERR_FILE_OUTPUT`
- `2005 ERR_INTERNAL`

All errors are non-fatal for per-lexeme failures: scanner emits
`CAT_NONRECOGNIZED` and continues.

## 5. Counter System

Implemented in `src/counter.c` with per-function partials and global totals.

Counters:
- `COUNTCOMP`
- `COUNTIO`
- `COUNTGEN`

Each update logs:
- input line number
- function name
- incremented counter amount
- partial in function
- global total

Compile-time gating:
- active only if `COUNTCONFIG == COUNT_ENABLED`
- and only in `OUTFORMAT_DEBUG`

## 6. Output Formats

### 6.1 RELEASE
- One line per non-empty input line.
- Tokens separated by one space.
- No empty output lines.

### 6.2 DEBUG
- RELEASE layout + input line number prefix.
- One blank line between token lines.
- Error/debug messages routed according to `DEBUG`.

## 7. API and Types

- `Token`, `TokenList` in `src/token.h`
- `run_lexer(const char *input_path, const char *output_path, TokenList *out_tokens)`
- `run_pipeline_with_optional_parser(...)` for scanner-to-parser chaining

## 8. P3 Hook

The scanner now has a parser hook function pointer (`ParserHookFn`) so the next
assignment can:
1. Continue in same process using in-memory tokens.
2. Still support loading `.cscn` in an independent parser executable.

## 9. Complexity / operation discussion

The dominant scanner cost is linear on input size (`O(n)`).

- Lexeme scanning: one pass by characters.
- DFA classification: per lexeme, proportional to lexeme length times number of
  DFA checks (short lexemes amortize well).
- Output generation: linear in rendered token text.

Measured operation counters allow comparing alternative scanner strategies (for
example, larger unified DFA vs ordered DFA list).

## 10. Validation

Automated acceptance checks are in:
- `tests/run_acceptance.sh`

They validate token categories, release/debug output behavior, non-recognized
handling, and count output generation.
