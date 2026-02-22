/**
 * @file token_lines.h
 * @brief File Tokens: Array of Line Tokens
 * @author Marc Bosch Manzano
 * @since 2026-02-14
 */

#ifndef TOKEN_LINES_H
#define TOKEN_LINES_H

#include "./token_line.h"

typedef struct TokenLines TokenLines;

TokenLines *init_token_lines(void);
void clear_token_lines(TokenLines *tokens);
void delete_token_lines(TokenLines **tokens);
void append_line_to_lines(TokenLines *tokens, TokenLine *line);
TokenLine *get_line_from_lines(const TokenLines *tokens, long position);
Token *get_token_from_lines(const TokenLines *tokens, long line, long column);
size_t get_num_lines_from_lines(const TokenLines *tokens);
size_t get_num_tokens_from_lines(const TokenLines *tokens);
char *to_token_lines_string(const TokenLines *tokens);
void print_token_lines(const TokenLines *tokens);

#endif /* SCANNER_H */
