/**
 * @title: Token Handling Functionalities
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "utils/error.h"

typedef enum {
    CAT_NUMBER,
    CAT_IDENTIFIER,
    CAT_KEYWORD,
    CAT_LITERAL,
    CAT_OPERATOR,
    CAT_SPECIALCHAR,
    CAT_NONRECOGNIZED
} TokenCategory;

static const char *const token_text[] = {
    [CAT_NUMBER] = "CAT_NUMBER",
    [CAT_IDENTIFIER] = "CAT_IDENTIFIER",
    [CAT_KEYWORD] = "CAT_KEYWORD",
    [CAT_LITERAL] = "CAT_LITERAL",
    [CAT_OPERATOR] = "CAT_OPERATOR",
    [CAT_SPECIALCHAR] = "CAT_SPECIALCHAR",
    [CAT_NONRECOGNIZED] = "CAT_NONRECOGNIZED"
};

typedef struct {
    char *lexeme;
    TokenCategory category;
    LineError error;
} Token;

typedef struct {
    char *lexeme;
    char *category;
} SimpleToken;

void print_token(void *token);

#endif // TOKEN_H