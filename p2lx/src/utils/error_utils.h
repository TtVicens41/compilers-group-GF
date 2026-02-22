/**
 * @file error_utils.h
 * @brief Error Handling Utilities.
 * @author Marc Bosch Manzano
 * @since 2026-01-30
 * @see Improved from P1 Preprocessor error_utils.h
 */

#ifndef ERRORS_HANDLING_H
#define ERRORS_HANDLING_H

#include <errno.h>

#include "../language_defs.h"

/**
 * @brief Compilation phase codes.
 */
typedef enum {
    PREPROCESSOR,
    LEXER,
    PARSER
} CompilationPhase;

/**
 * @brief Compiler error codes.
 * 
 * The elements of this enum are defined to be integer identifiers. The error
 * messages definition can be found in table `ERROR_MESSAGES`.
 */
typedef enum {
    SUCCESS = 0,
    ERR_FILE_NOT_FOUND = 100,
    ERR_NON_RECOGNIZED = 101,
    ERR_UNTERMINATED_LITERAL = 102,
    ERR_IDENTIFIER_NOT_DEFINED = 103,
    ERR_IDENTIFIER_TYPE_MISSING = 104,
    ERR_AUTOMATA_NOT_LOADED = 105,
    FILE_NOT_PROVIDED = 106
} ErrorCode;

/**
 * @brief Compiler messages.
 */
static const char *const ERROR_MESSAGES[] = {   
    [ENOENT] = "No such file or directory: %s\n",
    [EACCES] = "Permission denied: %s\n",
    [EISDIR] = "Is a directory: %s\n",
    [EMFILE] = "Too many open files: %s\n",
    [ENAMETOOLONG] = "Filename too long: %s\n",
    [FILE_NOT_PROVIDED] = "No input file name provided\n",
    [ERR_NON_RECOGNIZED] = "Unknown error", 
    [ERR_UNTERMINATED_LITERAL] = "Unterminated literal",   
    [ERR_IDENTIFIER_NOT_DEFINED] = "Not defined identifier", 
    [ERR_IDENTIFIER_TYPE_MISSING] = "Identifier type missing" 
};

/** Data structure for error message handling. */
typedef struct {
    char message[BUFFER_SIZE_SMALL];
    int line;
} Errors;

typedef struct {
    ErrorCode code;
    CompilationPhase phase;
    char *message;
} Error;

typedef struct {
    Error error;
    char *file_name;
} FileError;

typedef struct {
    FileError error;
    int line;
    int column;
    int *line_content;
} LineError;

/** 
 * @brief Static variable to store error messages. 
 */ 
static Errors errors;

/**
 * @brief Initializes an error copying to it a message and a line position.
 * @param message A read-only string.
 * @param line An integer.
 */
void init_error(const char *message, int line);

/**
 * @brief Tells user the reason that a file has failed to open.
 * @note The function works with `errno` from `errno.h` library
 */
void print_file_error(const char *path);

#endif // ERROR_H
