/**
 * @title: Lexical Analyzer Module
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/09
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

typedef struct {
    FILE *output_file;
    Token *tokens;
    int num_tokens;
} Lexer;


#endif // LEXER_H