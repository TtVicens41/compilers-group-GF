/**
 * Lexical Analyzer Module: Lexer
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/14
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "./lexer.h"

struct Lexer {
    DynamicPointerArray *tokens;
    int line_count;
};

Lexer *init_lexer(int line_count) {
    Lexer *lexer = calloc(1, sizeof(Lexer));
    if (!lexer) { return NULL; }

    lexer->tokens = init_dynamic_pointer_array();
    lexer->line_count = line_count;

    return lexer;
}

void clear_lexer(Lexer *lexer) {
    if (!lexer) { return; }
    hard_delete_dynamic_pointer_array(
        &(lexer->tokens), 
        (void (*)(void **))delete_token
    );
}

void delete_lexer(Lexer **lexer) {
    if (!*lexer) { return; }
    clear_lexer(*lexer);
    free(*lexer);
    *lexer = NULL;
}

void append_token_to_lexer(Lexer *lexer, Token *token) {
    if (!lexer) { return; }
    append_pointer_to_array(lexer->tokens, (void *)token);
}

Token *get_token_from_lexer(const Lexer *lexer, long position) {
    if (!lexer) { return NULL; }
    return (Token *)get_pointer_from_array(lexer->tokens, position);
}

size_t get_num_tokens(const Lexer *lexer) {
    if (!lexer) { return 0; }
    return get_num_pointers(lexer->tokens);
}

char *to_lexer_string(const Lexer *lexer) {
    if (!lexer) { return NULL; }
    
    char *str_lexer = "";

#if (DEBUG == 1) 
    char heading[BUFFER_SIZE_TINY];
    sprintf(heading, "[line=%d] ", lexer_cast->line_count);
    str_lexer = concat_strings(str_lexer, heading);
#endif

    const size_t num_tokens = get_num_tokens(lexer);

    for (size_t i = 0; i < num_tokens; i++) {
        Token *token = get_token_from_lexer(lexer, i);
        char *str_token = to_token_string(token);
        str_lexer = concat_strings(str_lexer, str_token);
    }

    return concat_strings(str_lexer, "\n");
}

void print_lexer(const Lexer *lexer) {
    if (!lexer) { return; }
    printf("%s", to_lexer_string(lexer));
}
