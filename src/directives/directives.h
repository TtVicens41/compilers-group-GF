/**
 * @title: Directives Directive Processing.
 * @brief: Declarations of arguments parsing utilities.
 * @authors: Davi Penna-Mattos & Pau Puig Guillén
 * @creation: before 2026/01/27
 */

#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include "context.h"

/*
 * process_directive
 * -----------------
 * Detects and dispatches a preprocessor directive.
 * Returns true if the line was a directive.
 */
bool process_directive(PreprocessorContext *ctx, const char *line);

#endif
