/**
 * @title: String Utilities for P3 Bottom-Up Parser
 */

#ifndef P3_STRING_UTILS_H
#define P3_STRING_UTILS_H

#include <stddef.h>

char *get_copy(const char *str);
void trim_whitespace(char *str);
int is_blank_line(const char *str);

#endif /* P3_STRING_UTILS_H */
