/**
 * @file scanner.c
 * @brief Lexical Analyzer Module: Scanner
 * @author Marc Bosch Manzano
 * @since 2026-02-08
 */

#include <string.h>

#include "./scanner.h"

TokenLines *scan_string(NFA *automaton, const char *string) {
    if (!automaton || !string) { return NULL; }

    const int string_length = (int)strlen(string);
    int line_count = 1;
    int column_count = 1;
    int start = 0;
    TokenLines *scanner = init_token_lines();
    TokenLine *lexer = init_token_line(line_count);

    reset_nfa(automaton);
    for (int end = 0; end < string_length; end++) {
        char current_char = string[end];
        char look_ahead_char = string[end + 1];
        
        if (current_char == CHAR_NEWLINE) {
            line_count++;
            if (get_num_tokens_from_line(lexer) > 0) {
                append_line_to_lines(scanner, lexer);
                lexer = init_token_line(line_count);
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
                
                Token *token = init_token(
                    lexeme, 
                    automatons[i]->category, 
                    column_count - ((end + 1) - start)
                );
                
                append_token_to_line(lexer, token);

                free(substring);
                free(lexeme);
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

#ifdef DEPRECATED
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
    TokenLine *lexer,
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

    append_token_to_line(lexer, token);
    return TRUE;
}

TokenLines *scan_string(NFA *automaton, const char *string) {
    if (!automaton || !string) { return NULL; }

    const int string_length = (int)strlen(string);
    int line_count = 1;
    int column_count = 1;
    int index = 0;
    TokenLines *scanner = init_token_lines();
    LineToken *lexer = init_token_line(line_count);

    if (!scanner || !lexer) {
        delete_token_lines(&scanner);
        delete_token_line(&lexer);
        return NULL;
    }

    while (index < string_length) {
        char current_char = string[index];

        if (current_char == CHAR_NEWLINE) {
            line_count++;
            if (get_num_tokens_from_line(lexer) > 0) {
                append_line_to_lines(scanner, lexer);
                lexer = init_token_line(line_count);
                if (!lexer) {
                    delete_token_lines(&scanner);
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
                        delete_token_lines(&scanner);
                        delete_token_line(&lexer);
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
                    delete_token_lines(&scanner);
                    delete_token_line(&lexer);
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
                delete_token_lines(&scanner);
                delete_token_line(&lexer);
                return NULL;
            }
            index = nonrec_end + 1;
            column_count += token_length;
        }        
    }

    if (get_num_tokens_from_line(lexer) > 0) {
        append_line_to_lines(scanner, lexer);
    } else {
        delete_token_line(&lexer);
    }
    
    return scanner;
}
#endif
