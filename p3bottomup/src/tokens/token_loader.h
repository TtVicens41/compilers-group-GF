/**
 * @file token_loader.h
 * @brief Load a TokenList from a .cscn scan file.
 */

#ifndef TOKEN_LOADER_H
#define TOKEN_LOADER_H

#include "token_list.h"
#include "../language/language.h"

/**
 * Load tokens from a .cscn file.
 *
 * File format – one token per line:
 *     TERMINAL_NAME LEXEME
 *
 * TERMINAL_NAME is matched against the Language symbol table to obtain the
 * numeric id stored in Token.type. The last effective token should be the
 * end-of-input marker (conventionally "$").
 *
 * @param path  Path to the .cscn file.
 * @param lang  Language used to resolve terminal names to ids.
 * @return  A heap-allocated TokenList, or NULL on failure.
 */
TokenList *token_loader_load_from_file(const char *path,
                                       const Language *lang);

#endif /* TOKEN_LOADER_H */
