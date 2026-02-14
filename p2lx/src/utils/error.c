/**
 * @title: error.c
 * @authors: Joan Vicente, Pau Puig
 * @creation:
 */

#include "error.h"

const char *error_message_text(ErrorCode code) {
    switch (code) {
        case ERR_NON_RECOGNIZED:
            return "Non-recognized lexeme";
        case ERR_UNTERMINATED_LITERAL:
            return "Unterminated literal";
        case ERR_FILE_INPUT:
            return "Cannot open input file";
        case ERR_FILE_OUTPUT:
            return "Cannot open output file";
        case ERR_INTERNAL:
            return "Internal error";
        case ERR_OK:
        default:
            return "No error";
    }
}
