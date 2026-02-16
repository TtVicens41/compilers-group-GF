/**
 * Lexical Analyzer Module: Scanner
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/14
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "./scanner.h"

struct Scanner {
    DynamicPointerArray *lexers;
};

Scanner *init_scanner(void) {
    Scanner *scanner = calloc(1, sizeof(Scanner));
    if (!scanner) {
        return NULL; 
    }
    scanner->lexers = init_dynamic_pointer_array();
    return scanner;
}

void append_lexer_to_scanner(Scanner *scanner, Lexer *lexer) {
    if (!scanner) { return; }
    append_pointer_to_array(scanner->lexers, (void *)lexer);
}

Lexer *get_lexer_from_scanner(const Scanner *scanner, long position) {
    if (!scanner) { return NULL; }
    return (Lexer *)get_pointer_from_array(scanner->lexers, position);
}

size_t get_num_lexers(const Scanner *scanner) {
    if (!scanner) { return 0; }
    return get_num_pointers(scanner->lexers);
}

size_t get_total_num_tokens(const Scanner *scanner) {
    if (!scanner) { return 0; }

    DynamicPointerArray *lexers = scanner->lexers;
    const size_t num_lexers = get_num_pointers(lexers);
    size_t num_tokens = 0;
    
    for (size_t i = 0; i < num_lexers; i++) {
        Lexer *lexer = (Lexer *)get_pointer_from_array(lexers, i);
        num_tokens += get_num_tokens(lexer);
    }
    
    return num_tokens;
}

char *get_formatted_scanner(void *scanner) {
    if (!scanner) { return NULL; }

    Scanner *scanner_cast = (Scanner *)scanner;
    const size_t num_lexers = get_num_lexers(scanner_cast);
    char *str_scanner = "";
    
    for (size_t i = 0; i < num_lexers; i++) {
        Lexer *lexer = get_lexer_from_scanner(scanner_cast, i);
        char *str_lexer = get_formatted_lexer((void *)lexer);
        str_scanner = concat_strings(str_scanner, str_lexer);
    }

    return str_scanner;
}

void clear_scanner(Scanner *scanner) {
    if (!scanner) { return; }
    hard_delete_dynamic_pointer_array(
        &(scanner->lexers), 
        (void (*)(void **))delete_lexer
    );
}

void delete_scanner(Scanner **scanner) {
    if (!*scanner) { return; }
    clear_scanner(*scanner);
    free(*scanner);
    *scanner = NULL;
}

void print_scanner(const Scanner *scanner) {
    if (!scanner) { return; }
    print_pointer_values(scanner->lexers, print_formatted_lexer);
}

Scanner *scan_string(NFA *automaton, const char *string) {
    if (!automaton || !string) { return NULL; }

    const int string_length = (int)strlen(string);
    int line_count = 1;
    int column_count = 1;
    int start = 0;
    Scanner *scanner = init_scanner();
    Lexer *lexer = init_lexer(line_count);

    reset_nfa(automaton);
    for (int end = 0; end < string_length; end++) {
        char current_char = string[end];
        char look_ahead_char = string[end + 1];
        
        if (current_char == CHAR_NEWLINE) {
            line_count++;
            if (get_num_tokens(lexer) > 0) {
                append_lexer_to_scanner(scanner, lexer);
                lexer = init_lexer(line_count);
            }
        }

        update_column_count(&column_count, current_char);

        consume_char_nfa(automaton, current_char);
        
        if (!is_alive_nfa(automaton)) {
            start = end + 1;
            reset_nfa(automaton);
            continue;
        }

        int size = automaton->size;
        DFA **automatons = automaton->automatons;

        for (int i = 0; i < size; i++) {
            if (
                check_accepting_condition_dfa(automatons[i])
                && !is_look_ahead_alive_dfa(automatons[i], look_ahead_char)
            ) {      
                char *substring = get_substring(string, start, end);
                char *lexeme = get_raw_string(substring);
                free(substring);

                SimpleToken *token = init_token(
                    lexeme, 
                    automatons[i]->category, 
                    column_count - ((end + 1) - start)
                );
                free(lexeme);
                
                append_token_to_lexer(lexer, token);

                start = end + 1;
                reset_nfa(automaton);
                break;
            }
        }

        if (!is_look_ahead_alive_nfa(automaton, look_ahead_char)) {
            start = end + 1;
            reset_nfa(automaton);
        }        
    }
    
    return scanner;
}