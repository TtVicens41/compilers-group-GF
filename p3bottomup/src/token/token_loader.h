/**
 * @title: Token Loader Module for P3 Bottom-Up Parser
 */

#ifndef P3_TOKEN_LOADER_H
#define P3_TOKEN_LOADER_H

#include "token.h"
#include "../language/language.h"

int load_tokens_from_cscn_tsv(const char *path, TokenStream *stream, const LanguageSpec *spec);
int map_scanner_token_to_terminal(Token *tok, const LanguageSpec *spec);

#endif /* P3_TOKEN_LOADER_H */
