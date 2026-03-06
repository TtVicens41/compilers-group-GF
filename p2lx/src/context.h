/**
 * @file context.h
 * @brief Lexer Context
 * @author Marc Bosch Manzano
 * @note Reviewed by Joan Vicente Martín
 * @since 2026-06-16
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Lexer context data structure.
 *
 * Holds all shared state required by the lexical analyzer engine. This structure
 * is passed to all modules to avoid global state and to allow extensibility 
 * of the engine.
 */
typedef struct {
    char *input_file;
    char *input_file_str;
    char *automata_file;
    char *automata_file_str;
    char *categories_file;
    char *categories_file_str;
    char *output_file;
    char *output_file_str;
} LexerContext;

static inline void clear_context(LexerContext *ctx) {
    free(ctx->input_file);
    free(ctx->input_file_str);
    free(ctx->categories_file);
    free(ctx->categories_file_str);
    free(ctx->automata_file);
    free(ctx->automata_file_str);
    free(ctx->output_file_str);
    free(ctx->output_file);
}

#endif
