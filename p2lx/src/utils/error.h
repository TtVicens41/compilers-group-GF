/**
 * @title: error.h
 * @authors:
 * @creation:
 */

#ifndef ERROR_H
#define ERROR_H

typedef enum {
    STEP_PREPROCESSOR = 1,
    STEP_LEXER = 2,
    STEP_PARSER = 3
} CompilationStep;

typedef enum {
    ERR_OK = 0,
    ERR_NON_RECOGNIZED = 2001,
    ERR_UNTERMINATED_LITERAL = 2002,
    ERR_FILE_INPUT = 2003,
    ERR_FILE_OUTPUT = 2004,
    ERR_INTERNAL = 2005
} ErrorCode;

const char *error_message_text(ErrorCode code);

#endif
