/**
 * @file token_line.h
 * @brief Line Tokens: Array of Column Tokens
 * @author Marc Bosch Manzano
 * @since 2026-02-19
 */

#ifndef TOKEN_LINE_H
#define TOKEN_LINE_H

#include "./token.h"

typedef struct TokenLine TokenLine;

TokenLine *init_token_line(size_t line_count);
void clear_token_line(TokenLine *tokens);
void delete_token_line(TokenLine **tokens);
void append_token_to_line(TokenLine *tokens, Token *token);
Token *get_token_from_line(const TokenLine *tokens, long position);
size_t get_num_tokens_from_line(const TokenLine *tokens);
size_t get_line_count_from_line(const TokenLine *tokens);
char *to_token_line_string(const TokenLine *tokens);
void print_token_line(const TokenLine *tokens);

#endif /* LINE_TOKENS */
