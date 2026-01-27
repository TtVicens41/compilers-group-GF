#ifndef DEFINE_H
#define DEFINE_H

#include "context.h"

/*
 * handle_define
 * -------------
 * Handles #define directive by parsing and storing the identifier
 * and its value in the symbol table.
 * 
 * Syntax: #define <identifier> <value>
 */
void handle_define(PreprocessorContext *ctx, const char *line);

/*
 * replace_defines_in_line
 * -----------------------
 * Replaces all occurrences of defined identifiers in a line
 * with their corresponding values from the symbol table.
 */
void replace_defines_in_line(PreprocessorContext *ctx, const char *line, char *output_line);

#endif // DEFINE_H
