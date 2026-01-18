#include "comments.h"
#include <string.h>

bool remove_comments(char *line, bool *in_block) {
    char *src = line;
    char *dst = line;

    while (*src) {
        if (*in_block) {
            if (src[0] == '*' && src[1] == '/') {
                *in_block = false;
                src += 2;
            } else {
                src++;
            }
        } else {
            if (src[0] == '/' && src[1] == '/') {
                break; // end of line
            }
            if (src[0] == '/' && src[1] == '*') {
                *in_block = true;
                src += 2;
                continue;
            }
            *dst++ = *src++;
        }
    }

    *dst = '\0';
    return true;
}
