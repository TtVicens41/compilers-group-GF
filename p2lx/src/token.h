/**
 * @title: Token Handling Functionalities
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
 */

#ifndef TOKEN
#define TOKEN

typedef enum {
    CAT_NUMBER,
    CAT_IDENTIFIER,
    CAT_KEYWORD,
    CAT_LITERAL,
    CAT_OPERATOR,
    CAT_SPECIALCHAR,
    CAT_NONRECOGNIZED
} TokenCategory;

#endif TOKEN