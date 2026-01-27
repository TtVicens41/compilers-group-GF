/**
 * @title: IFDEF Directive Processing.
 * @authors: Davi Panna-Mattos Dias de Paiva.
 * @creation: before 2026/01/27.
 */

#ifndef IFDEF_H
#define IFDEF_H

#include "context.h"

/*
 * handle_ifdef
 * ------------
 * Handles #ifdef directive by checking if an identifier is defined.
 * Disables output if the identifier is not in the symbol table.
 * Supports nested ifdef blocks.
 * 
 * Syntax: #ifdef <identifier>
 */
void handle_ifdef(PreprocessorContext *ctx, const char *line);

/*
 * handle_endif
 * ------------
 * Handles #endif directive by closing an ifdef block.
 * Restores output state if we're exiting a skipped block.
 */
void handle_endif(PreprocessorContext *ctx, const char *line);

#endif // IFDEF_H