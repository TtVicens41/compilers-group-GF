/**
 * @file token_list.c
 * @brief Linked-list container for tokens.
 */

#include <stdlib.h>

#include "token_list.h"
#include "../utils/string_utils.h"

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

char *token_list_string(const TokenList *list, int level) 
{
    if (!list) {
        return NULL;
    }
    char *s = NULL;
    int n = level + 1;
    jsonify_wrap(&s, level, 1, "{");
    jsonify(&s, n, 1, 1, "head", ptr_to_str(list->head));
    jsonify(&s, n, 0, 1, "tail", ptr_to_str(list->tail));
    jsonify_wrap(&s, level, 0, "}");
    return s;
}