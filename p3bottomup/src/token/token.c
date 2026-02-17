/**
 * @title: Token Module for P3 Bottom-Up Parser
 */

#include <stdlib.h>
#include <string.h>

#include "token.h"

TokenStream *create_token_stream(int initial_capacity) {
    TokenStream *stream;

    if (initial_capacity <= 0) {
        initial_capacity = 16;
    }

    stream = (TokenStream *)calloc(1, sizeof(TokenStream));
    if (!stream) {
        return NULL;
    }

    stream->items = (Token *)calloc(initial_capacity, sizeof(Token));
    if (!stream->items) {
        free(stream);
        return NULL;
    }

    stream->capacity = initial_capacity;
    stream->size = 0;
    stream->cursor = 0;

    return stream;
}

int push_token(TokenStream *stream, const Token *tok) {
    Token *next_items;

    if (!stream || !tok) {
        return FALSE;
    }

    if (stream->size >= stream->capacity) {
        int next_capacity = stream->capacity * 2;
        next_items = (Token *)realloc(stream->items, next_capacity * sizeof(Token));
        if (!next_items) {
            return FALSE;
        }
        stream->items = next_items;
        stream->capacity = next_capacity;
    }

    stream->items[stream->size++] = *tok;
    return TRUE;
}

Token *peek_token(TokenStream *stream) {
    if (!stream || stream->cursor < 0 || stream->cursor >= stream->size) {
        return NULL;
    }
    return &stream->items[stream->cursor];
}

Token *advance_token(TokenStream *stream) {
    Token *current = peek_token(stream);
    if (!current) {
        return NULL;
    }

    if (stream->cursor < stream->size) {
        stream->cursor++;
    }

    return current;
}

void append_eof_token(TokenStream *stream, int eof_symbol_id) {
    Token eof;

    if (!stream) {
        return;
    }

    memset(&eof, 0, sizeof(eof));
    strncpy(eof.lexeme, "$", TOKEN_LEXEME_MAX - 1);
    strncpy(eof.category, "EOF", TOKEN_CATEGORY_MAX - 1);
    eof.line = -1;
    eof.column = -1;
    eof.term_symbol_id = eof_symbol_id;

    push_token(stream, &eof);
}

void free_token_stream(TokenStream **stream) {
    if (!stream || !*stream) {
        return;
    }

    free((*stream)->items);
    (*stream)->items = NULL;
    (*stream)->size = 0;
    (*stream)->capacity = 0;
    (*stream)->cursor = 0;

    free(*stream);
    *stream = NULL;
}
