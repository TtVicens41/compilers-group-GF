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

#include "../arguments/arguments.h"

/**
 * Run the parser end-to-end.
 * @param ctx A read-write parser contxext to initialize the language, 
 * parse table, etc. The status field is set to EXIT_SUCCESS (0) 
 * EXIT_FAILURE (1).
 */
void parser_run(ParserContext *ctx);

#endif /* PARSER_H */
