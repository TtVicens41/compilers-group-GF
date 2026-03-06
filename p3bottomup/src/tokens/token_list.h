/**
 * @file token_list.h
 * @brief Linked list of tokens consumed by the parser.
 */

#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include "token.h"

typedef struct {
    Token *head;
    Token *tail;
} TokenList;

/** Allocate an empty token list. */
TokenList *token_list_create(void);

/** Append a new token to the end of the list. */
void token_list_add(TokenList *list, int type, const char *lexeme);

/** Free the list and every token it contains. */
void token_list_destroy(TokenList *list);

char *token_list_string(const TokenList *list, int level);

#endif /* TOKEN_LIST_H */
