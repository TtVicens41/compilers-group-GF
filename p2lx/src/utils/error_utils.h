/**
 * @title: error_utils.h
 * @authors:
 * @creation:
 */

#ifndef ERRORS_HANDLING_H
#define ERRORS_HANDLING_H

#include <stdio.h>

#include "error.h"

/**
 * @brief Describes the responsibility of `print_file_error` in the compiler pipeline.
 * @param Receives: `const char *path`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void print_file_error(const char *path);
void report_lexer_error(FILE *stream, ErrorCode code, int line, int column,
                        const char *lexeme, const char *extra);

#endif
