/**
 * @file error_utils.c
 * @brief Error Handling Utilities
 * @author Marc Bosch Manzano
 * @since 2026-02-16
 * @see Improved from P1 Preprocessor error_utils.c
 */

#include <string.h>
#include <stdio.h>

#include "error_utils.h"

void init_error(const char *message, int line) {
    strcpy(errors.message, message);
    errors.line = line;
}

void print_file_error(const char *path) {
    fprintf(stderr, ERROR_MESSAGES[errno], path);
}
