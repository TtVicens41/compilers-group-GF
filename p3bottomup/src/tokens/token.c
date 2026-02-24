/**
 * @file token.c
 * @brief Allocation and destruction of a single Token.
 */

#include <stdlib.h>
#include <string.h>

#include "token.h"

Token *token_create(int type, const char *lexeme)
{
    Token *t = calloc(1, sizeof(Token));
    if (!t) return NULL;

    t->type = type;
    if (lexeme) {
        strncpy(t->lexeme, lexeme, sizeof(t->lexeme) - 1);
        t->lexeme[sizeof(t->lexeme) - 1] = '\0';
    }
    t->next = NULL;
    return t;
}

void token_destroy(Token *token)
{
    free(token);
}
