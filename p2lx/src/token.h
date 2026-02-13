/**
 * @title: token.h
 * @authors:
 * @creation:
 */

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
/**
 * @brief Explica la responsabilidad de `token_category_from_text` en el flujo del compilador.
 * @param Recibe: `const char *text`.
 * @return Devuelve un valor de tipo `TokenCategory`.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
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

/**
 * @brief Explica la responsabilidad de `token_list_init` en el flujo del compilador.
 * @param Recibe: `TokenList *list`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void token_list_init(TokenList *list);
int token_list_push(TokenList *list, const char *lexeme, TokenCategory category,
                    int line, int column);
/**
 * @brief Explica la responsabilidad de `token_list_free` en el flujo del compilador.
 * @param Recibe: `TokenList *list`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void token_list_free(TokenList *list);

/**
 * @brief Explica la responsabilidad de `print_token` en el flujo del compilador.
 * @param Recibe: `const Token *token`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void print_token(const Token *token);

#endif
