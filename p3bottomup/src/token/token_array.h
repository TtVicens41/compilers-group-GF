/**
 * @brief A review from P3 Bottom Up token.h
 * @author Marc Bosch Manzano
 * @since 2026-02-19
 */

#ifndef P3_TOKEN_ARRAY_H
#define P3_TOKEN_ARRAY_H

#include "../language_defs.h"
#include "./token.h"

typedef struct _TokenStream _TokenStream;

_TokenStream *_create_token_stream(void);
void _push_token(_TokenStream *stream, const Token *tok);
Token *_peek_token(_TokenStream *stream);
Token *_advance_token(_TokenStream *stream);
void _append_eof_token(_TokenStream *stream, int eof_symbol_id);
void _free_token_stream(_TokenStream **stream);

#endif /* P3_TOKEN_H */
