/**
 * @file token_lines.c
 * @brief File Tokens: Array of Line Tokens
 * @author Marc Bosch Manzano
 * @since 2026-02-14
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "./token_lines.h"

struct TokenLines {
    DynamicPointerArray *lexers;
};

TokenLines *init_token_lines(void) {
    TokenLines *scanner = calloc(1, sizeof(TokenLines));
    if (!scanner) {
        return NULL; 
    }
    scanner->lexers = init_dynamic_pointer_array();
    return scanner;
}

void clear_token_lines(TokenLines *scanner) {
    if (!scanner) { return; }
    hard_delete_dynamic_pointer_array(
        &(scanner->lexers), 
        (void (*)(void **))delete_token_line
    );
}

void delete_token_lines(TokenLines **scanner) {
    if (!*scanner) { return; }
    clear_token_lines(*scanner);
    free(*scanner);
    *scanner = NULL;
}

void append_line_to_lines(TokenLines *scanner, TokenLine *lexer) {
    if (!scanner) { return; }
    append_pointer_to_array(scanner->lexers, (void *)lexer);
}

TokenLine *get_line_from_lines(const TokenLines *scanner, long position) {
    if (!scanner) { return NULL; }
    return (TokenLine *)get_pointer_from_array(scanner->lexers, position);
}

Token *get_token_from_lines(const TokenLines *scanner, long line, long column){
    if (!scanner) { return NULL; }
    return get_token_from_line(
        get_line_from_lines(scanner, line), column
    );
}

size_t get_num_lines_from_lines(const TokenLines *scanner) {
    if (!scanner) { return 0; }
    return get_num_pointers(scanner->lexers);
}

size_t get_num_tokens_from_lines(const TokenLines *scanner) {
    if (!scanner) { return 0; }

    DynamicPointerArray *lexers = scanner->lexers;
    const size_t num_lexers = get_num_pointers(lexers);
    size_t num_tokens = 0;
    
    for (size_t i = 0; i < num_lexers; i++) {
        TokenLine *lexer = (TokenLine *)get_pointer_from_array(lexers, i);
        num_tokens += get_num_tokens_from_line(lexer);
    }
    
    return num_tokens;
}

char *to_token_lines_string(const TokenLines *scanner) {
    if (!scanner) { return NULL; }

    char *str_scanner = get_copy("");
    const size_t num_lexers = get_num_lines_from_lines(scanner);
    
    for (size_t i = 0; i < num_lexers; i++) {
        TokenLine *lexer = get_line_from_lines(scanner, i);
        char *str_lexer = to_token_line_string(lexer);
        char *next = concat_strings(str_scanner, str_lexer);
        free(str_scanner);
        free(str_lexer);
        str_scanner = next;
    }

    return str_scanner;
}

void print_token_lines(const TokenLines *scanner) {
    if (!scanner) { return; }
    printf("%s", to_token_lines_string(scanner));
}
