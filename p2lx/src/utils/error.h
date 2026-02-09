/**
 * @title: Error Handling Functionalities
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
 */

#ifndef ERROR_H
#define ERROR_H

/**
 * @brief Compiler error codes.
 * 
 * The elements of this enum are defined to be integer identifiers. The error
 * messages definition can be found in table `ERROR_MESSAGES`.
 */
typedef enum {
    ERR_NON_RECOGNIZED,
    ERR_UNTERMINATED_LITERAL,
    ERR_IDENTIFIER_NOT_DEFINED,
    ERR_IDENTIFIER_TYPE_MISSING
} ErrorCode;

/**
 * @brief Compiler messages.
 */
static const char *const ERROR_MESSAGES[] = {   
   [ERR_NON_RECOGNIZED] = "Unknown error", 
   [ERR_UNTERMINATED_LITERAL] = "Unterminated literal",   
   [ERR_IDENTIFIER_NOT_DEFINED] = "Not defined identifier", 
   [ERR_IDENTIFIER_TYPE_MISSING] = "Identifier type missing"
};

typedef enum {
    PREPROCESSOR,
    LEXER,
    PARSER
} CompilationPhase;

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

#endif // ERROR_H
