/**
 * @title: Include Directive Processing.
 * @authors: Joan Vicente Martín.
 * @creation: before 2026/01/27.
 */

#ifndef INCLUDE_H
#define INCLUDE_H

#include "../context.h"
#include "../language_defs.h"

/**
 * Handles the `#include` directive using the context of the preprocessor.
 * Works by a given line of a file.
 * @param ctx A pointer to preprocessor context.
 * @param line A string of a line.
 */
void handle_include(PreprocessorContext *ctx, const char *line);

#endif
