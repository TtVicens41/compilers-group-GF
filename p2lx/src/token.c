/**
 * @title: token.c
 * @authors:
 * @creation:
 */

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

/**
 * @brief Describes the responsibility of `token_category_from_text` in the compiler pipeline.
 * @param Receives: `const char *text`.
 * @return Returns a value of type `TokenCategory`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
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

/**
 * @brief Describes the responsibility of `token_list_init` in the compiler pipeline.
 * @param Receives: `TokenList *list`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
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

/**
 * @brief Describes the responsibility of `token_list_free` in the compiler pipeline.
 * @param Receives: `TokenList *list`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
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

/**
 * @brief Describes the responsibility of `print_token` in the compiler pipeline.
 * @param Receives: `const Token *token`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void print_token(const Token *token) {
    if (!token) {
        return;
    }

    printf("<%s, %s>\n", token->lexeme, token_category_text(token->category));
}
