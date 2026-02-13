#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "utils/string_utils.h"

static const char *const TOKEN_TEXT[] = {
    [CAT_NUMBER] = "CAT_NUMBER",
    [CAT_IDENTIFIER] = "CAT_IDENTIFIER",
    [CAT_KEYWORD] = "CAT_KEYWORD",
    [CAT_LITERAL] = "CAT_LITERAL",
    [CAT_OPERATOR] = "CAT_OPERATOR",
    [CAT_SPECIALCHAR] = "CAT_SPECIALCHAR",
    [CAT_NONRECOGNIZED] = "CAT_NONRECOGNIZED"
};

const char *token_category_text(TokenCategory category) {
    if (category < CAT_NUMBER || category > CAT_NONRECOGNIZED) {
        return "CAT_NONRECOGNIZED";
    }
    return TOKEN_TEXT[category];
}

TokenCategory token_category_from_text(const char *text) {
    int i;
    if (!text) {
        return CAT_NONRECOGNIZED;
    }

    for (i = CAT_NUMBER; i <= CAT_NONRECOGNIZED; i++) {
        if (strcmp(text, TOKEN_TEXT[i]) == 0) {
            return (TokenCategory)i;
        }
    }

    return CAT_NONRECOGNIZED;
}

void token_list_init(TokenList *list) {
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
}

int token_list_push(TokenList *list, const char *lexeme, TokenCategory category,
                    int line, int column) {
    Token *grown;

    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 16 : list->capacity * 2;
        grown = (Token *)realloc(list->items, new_capacity * sizeof(Token));
        if (!grown) {
            return 0;
        }
        list->items = grown;
        list->capacity = new_capacity;
    }

    list->items[list->size].lexeme = get_copy(lexeme);
    if (!list->items[list->size].lexeme) {
        return 0;
    }
    list->items[list->size].category = category;
    list->items[list->size].line = line;
    list->items[list->size].column = column;
    list->size++;
    return 1;
}

void token_list_free(TokenList *list) {
    size_t i;

    if (!list) {
        return;
    }

    for (i = 0; i < list->size; i++) {
        free(list->items[i].lexeme);
    }

    free(list->items);
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
}

void print_token(const Token *token) {
    if (!token) {
        return;
    }

    printf("<%s, %s>\n", token->lexeme, token_category_text(token->category));
}
