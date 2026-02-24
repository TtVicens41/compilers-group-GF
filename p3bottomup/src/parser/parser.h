/**
 * @file parser.h
 * @brief High-level parse entry point.
 *
 * Assembles all components (Language, ParseTable, DFA, Stack, SRA,
 * TokenList, Logger) and drives the parse.  Owns the lifecycle of
 * every component it creates.
 */

#ifndef PARSER_H
#define PARSER_H

/**
 * Run the parser end-to-end.
 *
 * @param language_file     Path to the language definition file.
 * @param table_file        Path to the parse-table file.
 * @param token_file        Path to the .cscn token file.
 * @param output_file       Path for the debug-output file (may be NULL
 *                          to derive from token_file automatically).
 * @return 0 on successful parse (ACCEPT), non-zero otherwise.
 */
int parser_run(const char *language_file,
               const char *table_file,
               const char *token_file,
               const char *output_file);

#endif /* PARSER_H */
