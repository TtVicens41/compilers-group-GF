#ifndef ERRORS_HANDLING_H
#define ERRORS_HANDLING_H

#include <stdio.h>

#include "error.h"

void print_file_error(const char *path);
void report_lexer_error(FILE *stream, ErrorCode code, int line, int column,
                        const char *lexeme, const char *extra);

#endif
