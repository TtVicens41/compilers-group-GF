/**
 * @brief A review from P3 Bottom Up token.c
 * @author Marc Bosch Manzano
 * @since 2026-02-19
 */

#include <stdlib.h>

#include "./token_array.h"
#include "../utils/dynamic_pointer_array.h"
#include "../utils/string_utils.h"


/**
 * @author Marc Bosch Manzano
 * Take full advantage of dynamic array structures whenever you can.
 */
struct _TokenStream {
    DynamicPointerArray *items;
    size_t cursor;
};

_TokenStream *_create_token_stream(void) {
    _TokenStream *stream = (_TokenStream *)calloc(1, sizeof(Token));

    if (!stream) { 
        return NULL; 
    }

    stream->items = init_dynamic_pointer_array();
    return stream;
}

void _push_token(_TokenStream *stream, const Token *tok) {
    if (!stream) { 
        return; 
    }

    append_pointer_to_array(stream->items, (void *)tok);
}

Token *_peek_token(_TokenStream *stream) {
    if (!stream || stream->cursor < 0 || stream->cursor >= stream->size) {
        return NULL;
    }

    return (Token *)get_pointer_from_array(stream->items, stream->cursor);
}

Token *_advance_token(_TokenStream *stream) {
    Token *current = peek_token(stream);
    if (!current) {
        return NULL;
    }

    if (stream->cursor < stream->size) {
        stream->cursor++;
    }

    return current;
}

#define INVALID_LINE (-1)
#define INVALID_COLUMN INVALID_LINE

/**
 * @review_from: Marc Bosch Manzano
 * @warning Don not initialize `Token` in stack memory, since at
 * the end of the subroutine its address will be deleted. Vulnerable to
 * frees, since it cannot be freed a stack variable. For variables 
 * in stack memory, I recommend to not do memset of 0, it is better to 
 * do Token eof = { 0 }. The same for heap memory: calloc preferable 
 * than malloc, since initializes all fields to 0.
 */
Token *get_eof_token(int eof_symbol_id) {
    Token *eof = (Token *)calloc(1, sizeof(Token));
    
    if (!eof) {
        return NULL;
    }

    eof->lexeme = get_copy("$");
    eof->category = get_copy("EOF");
    eof->line = INVALID_LINE;
    eof->column = INVALID_COLUMN;
    eof->term_symbol_id = eof_symbol_id;
    return eof;
}

void _append_eof_token(_TokenStream *stream, int eof_symbol_id) {
    Token *eof = get_eof_token(eof_symbol_id);

    if (!stream || !eof) {
        return;
    }
    
    push_token(stream, eof);
}

/**
 * @review_from: Marc Bosch Manzano
 * A good practice I have seen here is not only do free, but set all
 * struct fields to zero or NULL, including the struct itzeld. 
 */
void _free_token_stream(_TokenStream **stream) {
    if (!stream || !*stream) {
        return;
    }

    soft_delete_dynamic_pointer_array(&(*stream)->items);
    (*stream)->size = 0;
    (*stream)->capacity = 0;
    (*stream)->cursor = 0;

    free(*stream);
    *stream = NULL;
}
