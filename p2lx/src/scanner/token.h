/**
 * @title: token.h
 * @authors: Pau Puig , Marc Bosch
 * @creation: 16/02/2025
 */


#ifndef TOKEN_H
#define TOKEN_H

#include "../language_defs.h"
#include "../automata/automata.h"

#define NON_RECOGNIZED_TOKEN "CAT_NON_RECOGNIZED"

typedef struct {
    char *lexeme;
    char *category;
    int column_count;
} Token;

Token *init_token(const char *lexeme, const char *category, int column_count);
Token *nonrecognized_token(const char *string);
Token *produce_token_from_string_dfa(DFA *automaton, const char *string);
Token *produce_token_from_string_nfa(NFA *automaton, const char *string);

void clear_token(Token *token);
void delete_token(Token **token);

void print_formatted_token(const Token *token);
char *to_token_string(const Token *token);

void update_line_count(int *line_count, char symbol);
void update_column_count(int *column_count, char symbol);

#endif // TOKEN_H
