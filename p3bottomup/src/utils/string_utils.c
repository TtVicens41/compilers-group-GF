/**
 * @title: String Utilities for P3 Bottom-Up Parser
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "string_utils.h"

char *get_copy(const char *str) {
    size_t len = strlen(str);
    char *copy = (char *)calloc(len + 1, sizeof(char));
    if (!copy) {
        return NULL;
    }
    memcpy(copy, str, len);
    copy[len] = '\0';
    return copy;
}

void trim_whitespace(char *str) {
    char *start = str;
    char *end;
    size_t len;

    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    len = strlen(str);
    if (len == 0) {
        return;
    }

    end = str + len - 1;
    while (end >= str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

int is_blank_line(const char *str) {
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}
