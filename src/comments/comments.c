#include "comments.h"
#include <string.h>

// Comment symbol definitions
#define BLOCK_COMMENT_START "/*"
#define BLOCK_COMMENT_END "*/"
#define LINE_COMMENT "//"

// Helper: Check if string starts with a specific symbol
static bool starts_with(const char *str, const char *symbol) {
    return strncmp(str, symbol, strlen(symbol)) == 0;
}

bool remove_comments(char *line, bool *in_block) {
    char *src = line;
    char *dst = line;

    while (*src) {
        if (*in_block) {
            // Look for block comment end symbol
            if (starts_with(src, BLOCK_COMMENT_END)) {
                *in_block = false;
                src += strlen(BLOCK_COMMENT_END);
            } else {
                src++;
            }
        } else {
            // Look for line comment symbol
            if (starts_with(src, LINE_COMMENT)) {
                break; // end of line
            }
            // Look for block comment start symbol
            if (starts_with(src, BLOCK_COMMENT_START)) {
                *in_block = true;
                src += strlen(BLOCK_COMMENT_START);
                continue;
            }
            *dst++ = *src++;
        }
    }

    *dst = '\0';
    return true;
}
