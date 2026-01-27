/**
 * @title: Comments Processing.
 * @authors: Davi Penna-Mattos, Pol Goicoechea Esparza, Marc Bosch Manzano.
 * @creation: before 2026/01/27
 */

#include <string.h>
#include "comments.h"


#include "../language_defs.h"

/// Helper: Check if string starts with a specific symbol
static bool starts_with(const char *str, const char *symbol) {
    return strncmp(str, symbol, strlen(symbol)) == 0;
}

void remove_comments(char *line, bool *in_block) {
    char *src = line;
    char *dst = line;

    while (*src) {
        if (*in_block) {
            // Look for block comment end symbol
            if (starts_with(src, COMMENT_BLOCK_END)) {
                *in_block = false;
                src += strlen(COMMENT_BLOCK_END);
            } else {
                src++;
            }
        } else {
            // Look for line comment symbol
            if (starts_with(src, COMMENT_LINE)) {
                break; // end of line
            }
            // Look for block comment start symbol
            if (starts_with(src, COMMENT_BLOCK_START)) {
                *in_block = true;
                src += strlen(COMMENT_BLOCK_START);
                continue;
            }
            *dst++ = *src++;
        }
    }

    *dst = '\0';
}
