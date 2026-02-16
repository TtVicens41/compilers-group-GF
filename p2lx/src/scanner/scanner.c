/**
 * @title: scanner.c
 * @authors: Pol Goicoechea
 * @creation: 16/02/2025
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "./scanner.h"

struct Scanner {
    DynamicPointerArray *lexers;
};

static int is_horizontal_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v';
}

static int is_special_or_operator_char(char c) {
    return c == '=' || c == '>' || c == '+' || c == '*' ||
           c == ';' || c == ',' || c == '(' || c == ')' ||
           c == '{' || c == '}' || c == '[' || c == ']';
}

static int is_token_separator(char c) {
    return c == '\0' || c == '\n' || is_horizontal_whitespace(c) ||
           is_special_or_operator_char(c) || c == '"';
}

static int consume_nonrecognized_end(const char *string, int length, int start) {
    int cursor = start;

    while (cursor < length) {
        char current = string[cursor];
        if (current == '\n' || is_horizontal_whitespace(current)) {
            break;
        }
        if (cursor > start && (is_special_or_operator_char(current) || current == '"')) {
            break;
        }
        cursor++;
    }

    if (cursor == start) {
        return start;
    }
    return cursor - 1;
}

static int force_nonrecognized_number(const DFA *accepted_dfa, char next_char) {
    if (!accepted_dfa || !accepted_dfa->category) {
        return FALSE;
    }
    if (strcmp(accepted_dfa->category, "CAT_NUMBER") != 0) {
        return FALSE;
    }

    if (next_char == '.' || isalpha((unsigned char)next_char) || next_char == '_') {
        return TRUE;
    }
    return FALSE;
}

static int append_token_from_range(
    Lexer *lexer,
    const char *string,
    int start,
    int end,
    const char *category,
    int column
) {
    char *substring = get_substring(string, start, end);
    char *lexeme;
    Token *token;
    if (!substring) {
        return FALSE;
    }

    lexeme = get_raw_string(substring);
    free(substring);
    if (!lexeme) {
        return FALSE;
    }

    token = init_token(lexeme, category, column);
    free(lexeme);
    if (!token) {
        return FALSE;
    }

    append_token_to_lexer(lexer, token);
    return TRUE;
}

Scanner *init_scanner(void) {
    Scanner *scanner = calloc(1, sizeof(Scanner));
    if (!scanner) {
        return NULL; 
    }
    scanner->lexers = init_dynamic_pointer_array();
    return scanner;
}

Scanner *scan_string(NFA *automaton, const char *string) {
    if (!automaton || !string) { return NULL; }

    const int string_length = (int)strlen(string);
    int line_count = 1;
    int column_count = 1;
    int index = 0;
    Scanner *scanner = init_scanner();
    Lexer *lexer = init_lexer(line_count);

    if (!scanner || !lexer) {
        delete_scanner(&scanner);
        delete_lexer(&lexer);
        return NULL;
    }

    while (index < string_length) {
        char current_char = string[index];

        if (current_char == CHAR_NEWLINE) {
            line_count++;
            if (get_num_tokens(lexer) > 0) {
                append_lexer_to_scanner(scanner, lexer);
                lexer = init_lexer(line_count);
                if (!lexer) {
                    delete_scanner(&scanner);
                    return NULL;
                }
            }
            column_count = 1;
            index++;
            continue;
        }

        if (is_horizontal_whitespace(current_char)) {
            column_count++;
            index++;
            continue;
        }

        {
            int start = index;
            int start_column = column_count;
            int end = index;
            int last_accept_end = -1;
            int last_accept_index = -1;
            int nonrec_end;
            int token_length;
            int size;
            DFA **automatons;

            reset_nfa(automaton);
            while (end < string_length) {
                int accepted_index = -1;
                char cursor = string[end];

                if (cursor == CHAR_NEWLINE) {
                    break;
                }

                consume_char_nfa(automaton, cursor);
                if (!is_alive_nfa(automaton)) {
                    break;
                }

                size = automaton->size;
                automatons = automaton->automatons;
                for (int i = 0; i < size; i++) {
                    if (check_accepting_condition_dfa(automatons[i])) {
                        accepted_index = i;
                        break;
                    }
                }

                if (accepted_index >= 0) {
                    last_accept_end = end;
                    last_accept_index = accepted_index;
                }
                end++;
            }

            if (last_accept_end >= start) {
                DFA *accepted_dfa = automaton->automatons[last_accept_index];
                char next_char = '\0';

                if (last_accept_end + 1 < string_length) {
                    next_char = string[last_accept_end + 1];
                }

                if (force_nonrecognized_number(accepted_dfa, next_char)) {
                    nonrec_end = consume_nonrecognized_end(string, string_length, start);
                    token_length = nonrec_end - start + 1;
                    if (!append_token_from_range(
                        lexer,
                        string,
                        start,
                        nonrec_end,
                        NON_RECOGNIZED_TOKEN,
                        start_column
                    )) {
                        delete_scanner(&scanner);
                        delete_lexer(&lexer);
                        return NULL;
                    }

                    index = nonrec_end + 1;
                    column_count += token_length;
                    continue;
                }

                token_length = last_accept_end - start + 1;
                if (!append_token_from_range(
                    lexer,
                    string,
                    start,
                    last_accept_end,
                    accepted_dfa->category,
                    start_column
                )) {
                    delete_scanner(&scanner);
                    delete_lexer(&lexer);
                    return NULL;
                }

                index = last_accept_end + 1;
                column_count += token_length;
                continue;
            }

            nonrec_end = consume_nonrecognized_end(string, string_length, start);
            token_length = nonrec_end - start + 1;
            if (!append_token_from_range(
                lexer,
                string,
                start,
                nonrec_end,
                NON_RECOGNIZED_TOKEN,
                start_column
            )) {
                delete_scanner(&scanner);
                delete_lexer(&lexer);
                return NULL;
            }
            index = nonrec_end + 1;
            column_count += token_length;
        }        
    }

    if (get_num_tokens(lexer) > 0) {
        append_lexer_to_scanner(scanner, lexer);
    } else {
        delete_lexer(&lexer);
    }
    
    return scanner;
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

void append_lexer_to_scanner(Scanner *scanner, Lexer *lexer) {
    if (!scanner) { return; }
    append_pointer_to_array(scanner->lexers, (void *)lexer);
}

Lexer *get_lexer_from_scanner(const Scanner *scanner, long position) {
    if (!scanner) { return NULL; }
    return (Lexer *)get_pointer_from_array(scanner->lexers, position);
}

Token *get_token_from_scanner(const Scanner *scanner, long line, long column){
    if (!scanner) { return NULL; }
    return get_token_from_lexer(
        get_lexer_from_scanner(scanner, line), column
    );
}

size_t get_num_lexers_from_scanner(const Scanner *scanner) {
    if (!scanner) { return 0; }
    return get_num_pointers(scanner->lexers);
}

size_t get_num_tokens_from_scanner(const Scanner *scanner) {
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

char *to_scanner_string(const Scanner *scanner) {
    if (!scanner) { return NULL; }

    char *str_scanner = get_copy("");
    const size_t num_lexers = get_num_lexers_from_scanner(scanner);
    
    for (size_t i = 0; i < num_lexers; i++) {
        Lexer *lexer = get_lexer_from_scanner(scanner, i);
        char *str_lexer = to_lexer_string((void *)lexer);
        char *next = concat_strings(str_scanner, str_lexer);
        free(str_scanner);
        free(str_lexer);
        str_scanner = next;
    }

    return str_scanner;
}

void print_scanner(const Scanner *scanner) {
    if (!scanner) { return; }
    printf("%s", to_scanner_string(scanner));
}
