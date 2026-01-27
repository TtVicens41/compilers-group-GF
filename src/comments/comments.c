#include "comments.h"
#include <string.h>
/**
 * @brief Removes comments from a line of code.
 * @param line The line of code to process.
 * @param in_block Pointer to a boolean indicating if currently inside a block comment.
 * @return true if comments were removed.
 */
bool remove_comments(char *line, bool *in_block) {
    char *src = line;  
    char *dst = line;   
    while (*src) {
        if (*in_block) {           
            if (strcmp(src, STAR_SLASH, 2) == 0) {
                *in_block = false;
                src += 2;
            } else {
                src++;
            }
        } 
        else {
            if (strcmp(src, DOUBLE_SLASH, 2) == 0) {
                break;
            }
            if (strcmp(src, SLASH_STAR, 2) == 0) {
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