# P3 Bottom-Up Parser (`p3bottomup`)

This project implements a modular **bottom-up shift/reduce parser engine** in C.
The parser is generic: grammar symbols, productions, action table, and goto table are loaded from a language file.

## Build

```bash
cd /Users/joanvm/Desktop/Curso-25-26/Segundo\ Trimestre/Compilers/compilers-group-GF/p3bottomup
./build_local.sh
```

## Usage

```bash
./p3parser <input.cscn> <language.txt>
```

Example:

```bash
./p3parser ./test_files/g1_valid_1.cscn ./resources/language1.txt
```

## Input `.cscn` format

TSV, one token per line:

```text
lexeme<TAB>category
```

Example:

```text
12	CAT_NUMBER
+	CAT_OPERATOR
(	CAT_SPECIALCHAR
```

## Language file format

Sections:

- `SYMBOLS ... END_SYMBOLS`
- `PRODUCTIONS ... END_PRODUCTIONS`
- `ACTION_TABLE ... END_ACTION_TABLE`
- `GOTO_TABLE ... END_GOTO_TABLE`
- `START_SYMBOL <name>`
- `EOF_SYMBOL <name>`

Provided language files:

- `resources/language1.txt` (handout arithmetic grammar)
- `resources/language2.txt` (slides grammar `e -> e + (e) | int`)

## Output debug trace

The parser writes one debug line per shift/reduce/accept/error into:

```text
<input_stem>_p3dbg.txt
```

Format:

```text
STEP | OP | INPUT_POS | LOOKAHEAD | PREV_STATE | NEW_STATE | RULE | STACK
```

## Core modules

- `src/language`: grammar symbols, productions, tables loader
- `src/token`: token stream and `.cscn` loading/mapping
- `src/stack`: parser stack operations
- `src/parser`: shift/reduce engine
- `src/trace`: debug trace writer
- `src/utils`: generic file/string helpers
- `src/main.c`: CLI and orchestration

## Token mapping

- `CAT_NUMBER` -> `NUM`
- `CAT_OPERATOR` with `+` -> `+`
- `CAT_OPERATOR` with `*` -> `*`
- `CAT_SPECIALCHAR` with `(` -> `(`
- `CAT_SPECIALCHAR` with `)` -> `)`
- `CAT_KEYWORD` with `int` -> `int` (for `language2.txt`)
- any other token remains unmapped and causes syntactic rejection

## Test files

- `test_files/g1_valid_1.cscn`
- `test_files/g1_valid_2.cscn`
- `test_files/g1_invalid_1.cscn`
- `test_files/g1_invalid_2.cscn`
- `test_files/g2_valid_1.cscn`
- `test_files/g2_invalid_1.cscn`
