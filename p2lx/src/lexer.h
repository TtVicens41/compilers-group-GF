/**
 * @title: lexer.h
 * @authors:
 * @creation:
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#include "automata/automata.h"
#include "counter.h"
#include "token.h"
#include "utils/error.h"

typedef struct {
    FILE *output_file;
    FILE *debug_stream;
    FILE *count_stream;
    CounterState counter;
    NFA *nfa;
    TokenList tokens;
    int current_line;
} Lexer;

typedef int (*ParserHookFn)(const TokenList *tokens, const char *token_path);

/**
 * @brief Explica la responsabilidad de `run_lexer` en el flujo del compilador.
 * @param Recibe: `const char *input_path, const char *output_path, TokenList *out_tokens`.
 * @return Devuelve un valor de tipo `int`.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
int run_lexer(const char *input_path, const char *output_path, TokenList *out_tokens);
int run_pipeline_with_optional_parser(const char *input_path,
                                      ParserHookFn parser_hook,
                                      TokenList *out_tokens,
                                      char **generated_output_path);

char *build_scanner_output_path(const char *input_path);
char *build_count_output_path(const char *input_path);

#endif
