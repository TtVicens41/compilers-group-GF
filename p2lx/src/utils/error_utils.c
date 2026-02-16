/**
 * Error Handling Utilities.
 * 
 * @description: Implements error handling utilities.
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/16
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
