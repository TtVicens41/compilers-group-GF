/**
 * @title: Error Handling Utilities.
 * @author: Marc Bosch Manzano.
 * @creation: 10/01/2026.
 */

#ifndef ERRORS_HANDLING_H
#define ERRORS_HANDLING_H

#include <errno.h>

#include "../language_defs.h"

/// Preprocessor arguments error messages.
#define FILE_NOT_PROVIDED "No input file name provided\n"

/// File error messages.
#define ENOENT_MSG "No such file or directory: %s\n"
#define EACCES_MSG "Permission denied: %s\n"
#define EISDIR_MSG "Is a directory: %s\n"
#define EMFILE_MSG "Too many open files: %s\n"
#define ENAMETOOLONG_MSG "Filename too long: %s\n"

/// Data structure for error message handling.
typedef struct {
    char message[BUFFER_SIZE_SMALL];
    int line;
} Errors;

/// Static structure to store error messages.
static Errors errors;

/**
 * Initializes an error copying to it a message and a line position.
 * @param message A read-only string.
 * @param line An integer
 */
void init_error(const char *message, int line);

/**
 * Tells user the reason that a file has failed to open.
 * @note The function works with `errno` of `errno.h` library
 */
void print_file_error(const char *path);

#endif
