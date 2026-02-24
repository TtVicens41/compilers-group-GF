/**
 * @file token_list.c
 * @brief Linked-list container for tokens.
 */

#include <stdlib.h>

#include "token_list.h"

TokenList *token_list_create(void)
{
    TokenList *list = calloc(1, sizeof(TokenList));
    return list;
}

void token_list_add(TokenList *list, int type, const char *lexeme)
{
    if (!list) return;

    Token *t = token_create(type, lexeme);
    if (!t) return;

    if (!list->head) {
        list->head = t;
        list->tail = t;
    } else {
        list->tail->next = t;
        list->tail = t;
    }
}

void token_list_destroy(TokenList *list)
{
    if (!list) return;

    Token *cur = list->head;
    while (cur) {
        Token *next = cur->next;
        token_destroy(cur);
        cur = next;
    }
    free(list);
}
