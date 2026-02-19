/**
 * @file token.c
 * @brief Token Handling Functionalities
 * @author Marc Bosch Manzano
 * @since 2026-02-09
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./token.h"

struct Token {
    char *lexeme;
    char *category;
    size_t column_count;
};

Token *init_token(const char *lexeme, const char *category, int column_count) {
    Token *token = calloc(1, sizeof(Token));
    if (!token) { return NULL; }

    token->lexeme = get_copy(lexeme);
    token->category = get_copy(category);
    token->column_count = column_count;

    return token;
}

Token *nonrecognized_token(const char *lexeme) {
    Token *token = calloc(1, sizeof(Token));
    if (!token) { return NULL; }

    token->lexeme = get_copy(lexeme);
    token->category = get_copy(NON_RECOGNIZED_TOKEN);

    return token;
}

Token *produce_token_from_string_dfa(DFA *automaton, const char *string) {
    Token *token = nonrecognized_token(string);
    if (!token) { 
        return NULL; 
    }

    if (accept_string_dfa(automaton, string)) {
        free(token->category);
        token->category = get_copy(automaton->category);
    } 

    return token;
}

Token *produce_token_from_string_nfa(NFA *automaton, const char *string) {
    for (int i = 0; i < automaton->size; i++) {
        if (accept_string_dfa(automaton->automatons[i], string)) {
            return produce_token_from_string_dfa(automaton->automatons[i], string);
        }
    }

    return nonrecognized_token(string);
}

void clear_token(Token *token) {
    if (!token) { return; }

    free(token->category);
    token->category = NULL;

    free(token->lexeme);
    token->lexeme = NULL;

    token->column_count = 0;
}

size_t get_column_count_from_token(const Token *token) {
    if (!token) { return 0; }
    return token->column_count;
}

void delete_token(Token **token) {
    if (!*token) { return; }

    clear_token(*token);
    free(*token);
    *token = NULL;
}

char *to_token_string(const Token *token) {
    char *str_token = "";
    
#if (DEBUG == 1)
    char header[BUFFER_SIZE_TINY];
    sprintf(header, "[column=%d] ", token->column_count);
    str_token = concat_strings(str_token, header);
#endif

    char buffer[BUFFER_SIZE_XLARGE];
    sprintf(
        buffer,
        "<%s, %s> ",
        token->lexeme,
        token->category
    );

    return concat_strings(str_token, buffer);
}

void print_formatted_token(const Token *token) {
    printf("%s", to_token_string(token));
}

void update_column_count(int *column_count, char symbol) {
    (*column_count)++;
    if (symbol == CHAR_NEWLINE) {
        (*column_count) = 1;
    }
}

void update_line_count(int *line_count, char symbol) {
    if (symbol == CHAR_NEWLINE) {
        (*line_count)++;
    }
}
