/**
 * @title: Token Module for P3 Bottom-Up Parser
 */

#ifndef P3_TOKEN_H
#define P3_TOKEN_H

#include "../language_defs.h"

typedef struct {
    char lexeme[TOKEN_LEXEME_MAX];
    char category[TOKEN_CATEGORY_MAX];
    int line;
    int column;
    int term_symbol_id;
} Token;

typedef struct {
    Token *items;
    int size;
    int capacity;
    int cursor;
} TokenStream;

TokenStream *create_token_stream(int initial_capacity);
int push_token(TokenStream *stream, const Token *tok);
Token *peek_token(TokenStream *stream);
Token *advance_token(TokenStream *stream);
void append_eof_token(TokenStream *stream, int eof_symbol_id);
void free_token_stream(TokenStream **stream);

#endif /* P3_TOKEN_H */
