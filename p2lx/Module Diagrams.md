# Compiler Project Block Diagrams

This document summarizes the P2 lexical scanner architecture using **block diagrams** (not flow charts).
All function and type identifiers are taken directly from the code.

## 1. Module Block Diagram

```mermaid
graph TD
    A["main.c\nmain()"] --> B["lexer.c\nrun_pipeline_with_optional_parser()"]
    B --> C["lexer.c\nrun_lexer()"]
    C --> D["automata/automata.c\nread_union_nfa()"]
    C --> E["token.c\nTokenList API"]
    C --> F["counter.c\nCounterState API"]
    C --> G["utils/error_utils.c\nreport_lexer_error()"]
    C --> H["utils/file_utils.c\nread/write/check files"]
    D --> I["resources/automata.txt"]
```

## 2. Key Functionality Block Diagram (exact identifiers)

```mermaid
graph LR
    IN["input_path: const char*"] --> P0["run_pipeline_with_optional_parser(input_path, parser_hook, out_tokens, generated_output_path)\nreturns int"]
    P0 --> P1["build_scanner_output_path(input_path)\nreturns char*"]
    P1 --> P2["run_lexer(input_path, output_path, out_tokens)\nreturns int"]

    P2 --> L1["resolve_automata_path()\nreturns const char*"]
    P2 --> L2["read_union_nfa(file)\nreturns NFA*"]
    P2 --> L3["process_line(lexer, line)\nreturns int"]

    L3 --> L4["parse_literal(line, &i, len, &lexeme_out, &is_terminated)\nreturns int"]
    L3 --> L5["parse_generic_chunk(line, &i, len, &lexeme_out)\nreturns int"]
    L5 --> L6["classify_generic_chunk(lexer, lexeme)\nreturns TokenCategory"]
    L6 --> L7["classify_lexeme_nfa(nfa, lexeme)\nreturns TokenCategory"]
    L3 --> L8["push_token(...)\nreturns int"]
    L8 --> L9["token_list_push(list, lexeme, category, line, column)\nreturns int"]

    P2 --> OUT1["output_path (.cscn)"]
    P2 --> OUT2["out_tokens: TokenList"]
```

## 3. Data Structure Block Diagram

```mermaid
graph TD
    LEX["struct Lexer\n- FILE *output_file\n- FILE *debug_stream\n- FILE *count_stream\n- CounterState counter\n- NFA *nfa\n- TokenList tokens\n- int current_line"]

    TOKLIST["struct TokenList\n- Token *items\n- size_t size\n- size_t capacity"]
    TOKARR["Token[] array\n[item0 | item1 | ...]"]
    TOK["struct Token\n- char *lexeme\n- TokenCategory category\n- int line\n- int column"]

    NFA["struct NFA\n- DFA **automatas\n- int size"]
    DFAARR["DFA*[] array\n[dfa0 | dfa1 | ...]"]
    DFA["struct DFA\n- TokenCategory category\n- char *alphabet\n- int alphabet_size\n- int states_size\n- int initial_state\n- int *accepting_states\n- int accepting_states_size\n- int **transitions\n- char *char_map"]

    CNT["struct CounterState\n- long total_comp\n- long total_io\n- long total_gen\n- CounterFunctionRow rows[128]\n- FILE *stream\n- int enabled"]

    LEX --> TOKLIST
    TOKLIST --> TOKARR
    TOKARR --> TOK
    LEX --> NFA
    NFA --> DFAARR
    DFAARR --> DFA
    LEX --> CNT
```

### Key file-tracking variables (input/output control)
- Input handling:
  - `FILE *input` in `run_lexer()`
  - `char line[LINE_BUFFER_SIZE]` in `run_lexer()`
  - `int i`, `int len` in `process_line()`
- Output handling:
  - `lexer.output_file` (`FILE *`) in `run_lexer()`
  - `char *output_path` from `build_scanner_output_path()`
  - `char *line_render`, `size_t line_render_len` in `process_line()`
  - `TokenList out_tokens` propagated to caller

## 4. Input/Output Transformation Diagram by Applied Rule

> In P2 there are no preprocessor directives in scanner input; transformation is by lexical rule application.

```mermaid
graph LR
    A["Input line buffer\nchar line[]"] --> B["process_line()"]

    B --> C1["Rule: whitespace\nis_whitespace_char()\n=> skip"]
    B --> C2["Rule: operator/special\nis_operator_char()/is_special_char()\n=> CAT_OPERATOR/CAT_SPECIALCHAR"]
    B --> C3["Rule: literal\nparse_literal()\n=> CAT_LITERAL or CAT_NONRECOGNIZED"]
    B --> C4["Rule: generic chunk\nparse_generic_chunk()\nclassify_generic_chunk()\n=> CAT_NUMBER / CAT_IDENTIFIER / CAT_KEYWORD / CAT_NONRECOGNIZED"]

    C2 --> D["push_token()"]
    C3 --> D
    C4 --> D

    D --> E1["TokenList tokens (memory)"]
    D --> E2["line_render buffer"]
    E2 --> F["write_line_output()\n=> .cscn file"]

    B --> ERR["report_lexer_error() for lexical issues"]
```

This is intentionally a **block-level design**. It models modules, interfaces, structures, and data movement without instruction-level control-flow detail.
