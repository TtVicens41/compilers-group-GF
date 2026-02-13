#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum {
    CAT_NUMBER,
    CAT_IDENTIFIER,
    CAT_KEYWORD,
    CAT_LITERAL,
    CAT_OPERATOR,
    CAT_SPECIALCHAR,
    CAT_NONRECOGNIZED
} TokenCategory;

const char *token_category_text(TokenCategory category);
TokenCategory token_category_from_text(const char *text);

typedef struct {
    char *lexeme;
    TokenCategory category;
    int line;
    int column;
} Token;

typedef struct {
    Token *items;
    size_t size;
    size_t capacity;
} TokenList;

void token_list_init(TokenList *list);
int token_list_push(TokenList *list, const char *lexeme, TokenCategory category,
                    int line, int column);
void token_list_free(TokenList *list);

void print_token(const Token *token);

#endif
