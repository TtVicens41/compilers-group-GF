/**
 * @title: Error Handling Utilities.
 * @brief: Implements error handling utilities.
 * @author: Marc Bosch Manzano.
 * @creation: 10/01/2026.
 */

#include <string.h>
#include <stdio.h>

#include "errors_utils.h"

void init_error(const char *message, int line) {
    strcpy(errors.message, message);
    errors.line = line;
}

void print_file_error(const char *path) {
    switch (errno) {
        case ENOENT: printf(EONENT_MSG, path); break;
        case EACCES: printf(EACCES_MSG, path); break;
        case EISDIR: printf(EISDIR_MSG, path); break;
        case EMFILE: printf(EMFILE_MSG, path); break;
        case ENAMETOOLONG: printf(ENAMETOOLONG_MSG, path); break;   
        default: break;
    }
}
