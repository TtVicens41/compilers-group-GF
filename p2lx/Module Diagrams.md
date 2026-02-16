# Compiler Project Block Diagrams

This document summarizes the P2 lexical scanner architecture using **block diagrams** (not flow charts).
All function and type identifiers are taken directly from the code.

## 1. Module Block Diagram

```mermaid
graph TD
    A["main.c
    main()"] --> B["lexer.c
    run_pipeline_with_optional_parser()"]
    B --> C["lexer.c
    run_lexer()"]
    C --> D["automata/automata.c
    read_union_nfa()"]
    C --> E["token.c
    TokenList API"]
    C --> F["counter.c
    CounterState API"]
    C --> G["utils/error_utils.c
    report_lexer_error()"]
    C --> H["utils/file_utils.c
    read/write/check files"]
    D --> I["resources/automata.txt"]
```

## 2. Key Functionality Block Diagram (exact identifiers)

```mermaid
graph LR
    IN["input_path: const char*"] --> P0["run_pipeline_with_optional_parser(input_path, parser_hook, out_tokens, generated_output_path)
    returns int"]
    P0 --> P1["build_scanner_output_path(input_path)
    returns char*"]
    P1 --> P2["run_lexer(input_path, output_path, out_tokens)
    returns int"]

    P2 --> L1["resolve_automata_path()
    returns const char*"]
    P2 --> L2["read_union_nfa(file)
    returns NFA*"]
    P2 --> L3["process_line(lexer, line)
    returns int"]

    L3 --> L4["parse_literal(line, &i, len, &lexeme_out, &is_terminated)
    returns int"]
    L3 --> L5["parse_generic_chunk(line, &i, len, &lexeme_out)
    returns int"]
    L5 --> L6["classify_generic_chunk(lexer, lexeme)
    returns TokenCategory"]
    L6 --> L7["classify_lexeme_nfa(nfa, lexeme)
    returns TokenCategory"]
    L3 --> L8["push_token(...)
    returns int"]
    L8 --> L9["token_list_push(list, lexeme, category, line, column)
    returns int"]

    P2 --> OUT1["output_path (.cscn)"]
    P2 --> OUT2["out_tokens: TokenList"]
```

## 3. Data Structure Block Diagram

```mermaid
graph TD
    LEX["struct Lexer
    - FILE *output_file
    - FILE *debug_stream
    - FILE *count_stream
    - CounterState counter
    - NFA *nfa
    - TokenList tokens
    - int current_line"]

    TOKLIST["struct TokenList
    - Token *items
    - size_t size
    - size_t capacity"]
    TOKARR["Token[] array
    [item0 | item1 | ...]"]
    TOK["struct Token
    - char *lexeme
    - TokenCategory category
    - int line
    - int column"]

    NFA["struct NFA
    - DFA **automatas
    - int size"]
    DFAARR["DFA*[] array
    [dfa0 | dfa1 | ...]"]
    DFA["struct DFA
    - TokenCategory category
    - char *alphabet
    - int alphabet_size
    - int states_size
    - int initial_state
    - int *accepting_states
    - int accepting_states_size
    - int **transitions
    - char *char_map"]

    CNT["struct CounterState
    - long total_comp
    - long total_io
    - long total_gen
    - CounterFunctionRow rows[128]
    - FILE *stream
    - int enabled"]

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
    A["Input line buffer
    char line[]"] --> B["process_line()"]

    B --> C1["Rule: whitespace
    is_whitespace_char()
    => skip"]
    B --> C2["Rule: operator/special
    is_operator_char()/is_special_char()
    => CAT_OPERATOR/CAT_SPECIALCHAR"]
    B --> C3["Rule: literal
    parse_literal()
    => CAT_LITERAL or CAT_NONRECOGNIZED"]
    B --> C4["Rule: generic chunk
    parse_generic_chunk()
    classify_generic_chunk()
    => CAT_NUMBER / CAT_IDENTIFIER / CAT_KEYWORD / CAT_NONRECOGNIZED"]

    C2 --> D["push_token()"]
    C3 --> D
    C4 --> D

    D --> E1["TokenList tokens (memory)"]
    D --> E2["line_render buffer"]
    E2 --> F["write_line_output()
    => .cscn file"]

    B --> ERR["report_lexer_error() for lexical issues"]
```

This is intentionally a **block-level design**. It models modules, interfaces, structures, and data movement without instruction-level control-flow detail.
