/**
 * @title: String Utilities for P3 Bottom-Up Parser
 */

#ifndef P3_STRING_UTILS_H
#define P3_STRING_UTILS_H

#include <stddef.h>

/**
 * Copies a string into a new one.
 * @param[in] string A pointer to read-only character array.
 * @return A pointer to a dinamically allocated character array being its 
 * size the length of its characters.
 */
char *get_copy(const char *string);

/**
 * Trim leading and trailing whitespace in place.
 * @param[inout] str A pointer to read-write character array.
 */
void trim_whitespace(char *str);

/**
 * Detects a blank line
 * @param[in] str A pointer to a read-only character array.
 * @return A boolen that is 1 when a blank line is detected, 0 otherwise.
 */
int is_blank_line(const char *str);

#endif /* P3_STRING_UTILS_H */
