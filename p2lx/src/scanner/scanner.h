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
void clear_scanner(Scanner *scanner);
void delete_scanner(Scanner **scanner);
void append_lexer_to_scanner(Scanner *scanner, Lexer *lexer);
Lexer *get_lexer_from_scanner(const Scanner *scanner, long position);
size_t get_num_lexers(const Scanner *scanner);
size_t get_total_num_tokens(const Scanner *scanner);
char *get_formatted_scanner(void *scanner);
void print_scanner(const Scanner *scanner);

Scanner *scan_string(NFA *automaton, const char *string);
//Scanner *scan_string(NFA *automaton, const char *string);

#endif // SCANNER_H