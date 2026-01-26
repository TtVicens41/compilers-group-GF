#ifndef DEFINE_H
#define DEFINE_H

#include "context.h"

/*
 * handle_define
 * -------------
 * Handles #define directive by parsing and storing the identifier
 * in the symbol table.
 * 
 * Syntax: #define <identifier>
 */
void handle_define(PreprocessorContext *ctx, const char *line);

#endif // DEFINE_H
