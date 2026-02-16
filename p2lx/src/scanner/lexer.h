/**
 * Lexical Analyzer Module: Lexer
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#ifndef LEXER_H
#define LEXER_H

#include "./token.h"

typedef struct Lexer Lexer;
Lexer *init_lexer(int line_count);
void clear_lexer(Lexer *lexer);
void delete_lexer(Lexer **lexer);
void append_token_to_lexer(Lexer *lexer, SimpleToken *token);
SimpleToken *get_token_from_lexer(const Lexer *lexer, long position);
size_t get_num_tokens(const Lexer *lexer);
char *get_formatted_lexer(void *lexer);
void print_lexer(const Lexer *lexer);
void print_formatted_lexer(void *lexer);

#endif // LEXER_H