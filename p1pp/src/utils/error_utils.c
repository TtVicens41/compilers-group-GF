/**
 * @title: Error Handling Utilities.
 * @brief: Implements error handling utilities.
 * @author: Marc Bosch Manzano.
 * @creation: 10/01/2026.
 */

#include <string.h>
#include <stdio.h>

#include "error_utils.h"

void init_error(const char *message, int line) {
    strcpy(errors.message, message);
    errors.line = line;
}

void print_file_error(const char *path) {
    switch (errno) {
        case ENOENT: fprintf(stderr, ENOENT_MSG, path); break;
        case EACCES: fprintf(stderr, EACCES_MSG, path); break;
        case EISDIR: fprintf(stderr, EISDIR_MSG, path); break;
        case EMFILE: fprintf(stderr, EMFILE_MSG, path); break;
        case ENAMETOOLONG: fprintf(stderr, ENAMETOOLONG_MSG, path); break;   
        default: break;
    }
}
