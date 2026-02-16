/**
 * Token Handling Functionalities
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/09
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./token.h"
#include "../utils/utils.h"

SimpleToken *init_token(const char *lexeme, const char *category, int column_count) {
    SimpleToken *token = calloc(1, sizeof(SimpleToken));
    if (!token) { return NULL; }

    token->lexeme = get_copy(lexeme);
    token->category = get_copy(category);
    token->column_count = column_count;

    return token;
}

SimpleToken *nonrecognized_token(const char *lexeme) {
    SimpleToken *token = calloc(1, sizeof(SimpleToken));
    if (!token) { return NULL; }

    token->lexeme = get_copy(lexeme);
    token->category = get_copy(category_text[CAT_NONRECOGNIZED]);

    return token;
}

SimpleToken *produce_token_from_string_dfa(DFA *automaton, const char *string) {
    SimpleToken *token = nonrecognized_token(string);
    if (!token) { 
        return NULL; 
    }

    token->lexeme = get_copy(string);

    if (accept_string_dfa(automaton, string)) {
        free(token->category);
        token->category = get_copy(automaton->category);
    } 

    return token;
}

SimpleToken *produce_token_from_string_nfa(NFA *automaton, const char *string) {
    for (int i = 0; i < automaton->size; i++) {
        if (accept_string_dfa(automaton->automatons[i], string)) {
            return produce_token_from_string_dfa(automaton->automatons[i], string);
        }
    }

    return nonrecognized_token(string);
}

void clear_token(void *token) {
    SimpleToken *simple_token = (SimpleToken *)token;

    free(simple_token->category);
    simple_token->category = NULL;

    free(simple_token->lexeme);
    simple_token->lexeme = NULL;
}

void delete_token(void **token) {
    clear_token(*token);
    free(*token);
    *token = NULL;
}

void print_token(void *token) {
    SimpleToken *simple_token = (SimpleToken *)token;

    printf("{\n");
    printf("lexeme: %s\n", simple_token->lexeme);
    printf("category: %s,\n", simple_token->category);
    printf("}\n");
}

char *get_formatted_token(void *token) {
    SimpleToken *simple_token = (SimpleToken *)token;

    char *str_token = "";
    
#if (DEBUG == 1)
    char header[BUFFER_SIZE_TINY];
    sprintf(header, "[column=%d] ", simple_token->column_count);
    str_token = concat_strings(str_token, header);
#endif

    char buffer[BUFFER_SIZE_XLARGE];
    sprintf(
        buffer,
        "<%s, %s> ",
        simple_token->lexeme,
        simple_token->category
    );

    return concat_strings(str_token, buffer);
}

void print_formatted_token(void *token) {
    printf("%s", get_formatted_token(token));
}

int update_column_count(int *column_count, char symbol) {
    (*column_count)++;
    if (symbol == CHAR_NEWLINE) {
        (*column_count) = 1;
    }
}

int update_line_count(int *line_count, char symbol) {
    if (symbol == CHAR_NEWLINE) {
        (*line_count)++;
    }
}

