/**
 * Lexical Analyzer Module: Scanner
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/14
 */

#ifndef SCANNER_H
#define SCANNER_H

#include "./lexer.h"

typedef struct Scanner Scanner;
Scanner *init_scanner(void);
Scanner *scan_string(NFA *automaton, const char *string);
void clear_scanner(Scanner *scanner);
void delete_scanner(Scanner **scanner);
void append_lexer_to_scanner(Scanner *scanner, Lexer *lexer);
Lexer *get_lexer_from_scanner(const Scanner *scanner, long position);
Token *get_token_from_scanner(const Scanner *scanner, long line, long column);
size_t get_num_lexers_from_scanner(const Scanner *scanner);
size_t get_num_tokens_from_scanner(const Scanner *scanner);
char *to_scanner_string(const Scanner *scanner);
void print_scanner(const Scanner *scanner);

#endif // SCANNER_H