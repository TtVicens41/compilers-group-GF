/**
 * @file token_line.c
 * @brief Line Tokens: Array of Column Tokens
 * @author Marc Bosch Manzano
 * @since 2026-02-19
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "./token_line.h"

struct TokenLine {
    DynamicPointerArray *tokens;
    size_t line_count;
};

TokenLine *init_token_line(size_t line_count) {
    TokenLine *line = calloc(1, sizeof(TokenLine));
    if (!line) { return NULL; }

    line->tokens = init_dynamic_pointer_array();
    line->line_count = line_count;

    return line;
}

void clear_token_line(TokenLine *line) {
    if (!line) { return; }
    hard_delete_dynamic_pointer_array(
        &(line->tokens), 
        (void (*)(void **))delete_token
    );
}

void delete_token_line(TokenLine **line) {
    if (!*line) { return; }
    clear_token_line(*line);
    free(*line);
    *line = NULL;
}

void append_token_to_line(TokenLine *line, Token *token) {
    if (!line) { return; }
    append_pointer_to_array(line->tokens, (void *)token);
}

Token *get_token_from_line(const TokenLine *line, long position) {
    if (!line) { return NULL; }
    return (Token *)get_pointer_from_array(line->tokens, position);
}

size_t get_line_count_from_line(const TokenLine *line) {
    if (!line) { return 0; }
    return line->line_count;
}

size_t get_num_tokens_from_line(const TokenLine *line) {
    if (!line) { return 0; }
    return get_num_pointers(line->tokens);
}

char *to_token_line_string(const TokenLine *line) {
    if (!line) { return NULL; }
    
    char *str_lexer = "";

#if (DEBUG == 1) 
    char heading[BUFFER_SIZE_TINY];
    sprintf(heading, "[line=%d] ", line->line_count);
    str_lexer = concat_strings(str_lexer, heading);
#endif

    const size_t num_tokens = get_num_tokens_from_line(line);

    for (size_t i = 0; i < num_tokens; i++) {
        Token *token = get_token_from_line(line, i);
        char *str_token = to_token_string(token);
        str_lexer = concat_strings(str_lexer, str_token);
    }

    return concat_strings(str_lexer, "\n");
}

void print_token_line(const TokenLine *line) {
    if (!line) { return; }
    printf("%s", to_token_line_string(line));
}
