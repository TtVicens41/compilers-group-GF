/**
 * @title: lexer.h
 * @authors: Marc Bosch
 * @creation: 16/02/2026
 */


#ifndef LEXER_H
#define LEXER_H

#include "./token.h"

typedef struct Lexer Lexer;
Lexer *init_lexer(int line_count);
void clear_lexer(Lexer *lexer);
void delete_lexer(Lexer **lexer);
void append_token_to_lexer(Lexer *lexer, Token *token);
Token *get_token_from_lexer(const Lexer *lexer, long position);
size_t get_num_tokens(const Lexer *lexer);
char *to_lexer_string(const Lexer *lexer);
void print_lexer(const Lexer *lexer);

#endif // LEXER_H
