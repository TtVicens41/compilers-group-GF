/**
 * @title: error_utils.c
 * @authors: Marc Bosch
 * @creation: 16/02/2026
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
