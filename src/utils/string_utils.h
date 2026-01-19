/**
 * Made by Joan Vicente Martín (19/01/2026)
 * 
 * String utilities for the C preprocessor.
 * Contains common string manipulation functions used across modules.
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Checks if a character is a valid C identifier character (A-Za-z0-9_).
 * Useful for detecting word boundaries in token replacement.
 */
bool is_identifier_char(char c);

/**
 * Duplicates a string (portable strdup implementation).
 * Returns a newly allocated copy of the string.
 * Returns NULL on allocation failure or if input is NULL.
 * Caller is responsible for freeing the returned string.
 */
char *str_duplicate(const char *s);

/**
 * Trims trailing whitespace from a string in-place.
 * Modifies the string by placing null terminators.
 */
void str_trim_trailing(char *s);

/**
 * Skips leading whitespace in a string.
 * Returns pointer to first non-whitespace character.
 */
const char *str_skip_whitespace(const char *s);

#endif /* STRING_UTILS_H */
