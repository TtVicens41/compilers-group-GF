# p3bottomup – General Bottom-Up Shift/Reduce Parser

A table-driven, grammar-independent shift/reduce parser engine written in C.

## Overview

The parser implements a canonical **Shift / Reduce Automaton (SRA)** that:

- Uses a **stack + DFA** driven by **ACTION / GOTO tables**
- Loads the **grammar** and **parse table** entirely from data files
- Reads tokenised input from `.cscn` files
- Produces a detailed step-by-step **debug trace**
- Can switch to any SLR(1)-compatible grammar without modifying engine code

## Architecture

```
Application Layer          (main.c)
        ↓
Parser Layer               (parser.c, sra.c)
        ↓
Automaton Layer            (dfa.c, parse_table.c)
        ↓
Parsing Structures Layer   (stack.c)
        ↓
Language Layer             (language.c, grammar.c)
        ↓
Input Layer                (token.c, token_list.c, token_loader.c)
```

Each layer depends only on layers below it.  No circular dependencies.

## Directory Structure

```
p3bottomup/
├── build.sh
├── README.md
├── resources/
│   ├── grammar_a/          ← s→e, e→e+t|t, t→t*f|f, f→(e)|NUM
│   │   ├── language.txt
│   │   ├── parse_table.txt
│   │   ├── input.cscn
│   │   └── input2.cscn
│   └── grammar_b/          ← e→e+(e), e→int
│       ├── language.txt
│       ├── parse_table.txt
│       ├── input.cscn
│       └── input2.cscn
└── src/
    ├── main.c
    ├── parser/      sra.c  sra.h  parser.c  parser.h
    ├── automaton/   dfa.c  dfa.h  parse_table.c  parse_table.h
    ├── stack/       stack.c  stack.h
    ├── language/    language.c  language.h  grammar.c  grammar.h
    ├── tokens/      token.c token.h token_list.c token_list.h
    │                token_loader.c token_loader.h
    └── utils/       logger.c  logger.h
```

## Building

```bash
chmod +x build.sh
./build.sh
```

Produces the `parser` executable in the project root.

## Usage

```bash
./parser <language_file> <parse_table_file> <token_file> [output_file]
```

If `output_file` is omitted the debug trace is generated as
`<token_file_base>_p3dbg.txt`.

### Examples

**Grammar A** – arithmetic expressions (`3 + 5 * 2`):

```bash
./parser resources/grammar_a/language.txt \
         resources/grammar_a/parse_table.txt \
         resources/grammar_a/input.cscn
```

**Grammar B** – parenthesised additions (`int + ( int )`):

```bash
./parser resources/grammar_b/language.txt \
         resources/grammar_b/parse_table.txt \
         resources/grammar_b/input.cscn
```

## File Formats

### language.txt

```
SYMBOLS
T <id> <name>       ← terminal
N <id> <name>       ← nonterminal
...

PRODUCTIONS
<prod_id> <lhs_id> <rhs_len> <rhs_id_0> ... <rhs_id_n>
...

START
<start_symbol_id>
```

### parse_table.txt

```
STATES <n>
TERMINALS <n>
NONTERMINALS <n>

ACTION
<state> <terminal_id> <S|R|A> <value>
...

GOTO
<state> <nonterminal_id> <target_state>
...
```

- **S** value = target state
- **R** value = production id
- **A** = accept (value ignored)
- Entries not listed default to ERROR / -1

### .cscn (token file)

```
TERMINAL_NAME LEXEME
TERMINAL_NAME LEXEME
...
$ $
```

Lines starting with `#` are comments. The `$` token marks end-of-input.

## Debug Output

Each step of the parse produces a structured log entry:

```
STEP 5
  Operation:     SHIFT
  InputPos:      3
  PreviousState: 4
  NewState:      7
  Stack:         [0,?][4,e][7,+]
```

## Authors

Compiler group GF — UPF, 2025-2026.
