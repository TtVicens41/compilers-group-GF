/**
 * Made by Joan Vicente Martín (19/01/2026)
 * 
 * Implementation of string utilities.
 * Common string manipulation functions used across the preprocessor.
 */

#include "string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Checks if a character is a valid C identifier character.
 */
bool is_identifier_char(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
}

/**
 * Duplicates a string (portable strdup implementation).
 */
char *str_duplicate(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (copy != NULL) {
        memcpy(copy, s, len + 1);
    }
    return copy;
}

/**
 * Trims trailing whitespace from a string in-place.
 */
void str_trim_trailing(char *s) {
    if (s == NULL) return;
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[--len] = '\0';
    }
}

/**
 * Skips leading whitespace in a string.
 */
const char *str_skip_whitespace(const char *s) {
    if (s == NULL) return NULL;
    while (*s && isspace((unsigned char)*s)) {
        s++;
    }
    return s;
}
