/**
 * Token Handling Functionalities
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "../language_defs.h"
#include "../automata/automata.h"

typedef struct {
    char *lexeme;
    TokenCategory category;
    LineError error;
} Token;

typedef struct {
    char *lexeme;
    char *category;
    int column_count;
} SimpleToken;

SimpleToken *init_token(const char *lexeme, const char *category, int column_count);
SimpleToken *nonrecognized_token(const char *string);
SimpleToken *produce_token_from_string_dfa(DFA *automaton, const char *string);
SimpleToken *produce_token_from_string_nfa(NFA *automaton, const char *string);

void clear_token(void *token);
void delete_token(void **token);

void print_token(void *token);
void print_formatted_token(void *token);
char *get_formatted_token(void *token);

int update_column_count(int *column_count, char symbol);
int update_line_count(int *line_count, char symbol);

#endif // TOKEN_H