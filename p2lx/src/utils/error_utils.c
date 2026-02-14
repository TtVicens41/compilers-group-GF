/**
 * @title: error_utils.c
 * @authors:
 * @creation:
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "error_utils.h"

/**
 * @brief Describes the responsibility of `print_file_error` in the compiler pipeline.
 * @param Receives: `const char *path`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void print_file_error(const char *path) {
    if (!path) {
        return;
    }

    fprintf(stderr, "File error (%d): %s [%s]\n", errno, strerror(errno), path);
}

void report_lexer_error(FILE *stream, ErrorCode code, int line, int column,
                        const char *lexeme, const char *extra) {
    FILE *target = stream ? stream : stderr;

    fprintf(target,
            "[ERR step=%d code=%d line=%d col=%d] %s | lexeme=\"%s\"",
            STEP_LEXER, code, line, column, error_message_text(code),
            lexeme ? lexeme : "");

    if (extra && extra[0] != '\0') {
        fprintf(target, " | %s", extra);
    }

    fprintf(target, "\n");
}
