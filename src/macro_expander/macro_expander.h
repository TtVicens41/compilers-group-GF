/**
 * @title: Macro Expander.
 * @brief: Declarations of functions to process the macros. 
 * @authors: Davi Panna-Mattos Dias de Paiva¡.
 * @creation: before 2026/01/27.
 */

#ifndef MACRO_EXPANDER_H
#define MACRO_EXPANDER_H

#include <stddef.h>

#define MAX_MACRO_ARGS 10

/*
 * find_matching_paren
 * -------------------
 * Find the matching closing parenthesis, handling nesting.
 */
const char *find_matching_paren(const char *str);

/*
 * parse_macro_arguments
 * ---------------------
 * Parse arguments from a macro call.
 * Input: "MAX(5, 10)" starting after "MAX" -> Output: array of arguments
 * Returns: number of arguments parsed
 */
int parse_macro_arguments(const char *call, char args[][256], int max_args);

/*
 * expand_macro
 * ------------
 * Expand a macro by substituting parameters with arguments.
 * Parameters:
 *   - body: macro body template
 *   - params: comma-separated parameter names
 *   - args: array of argument values
 *   - arg_count: number of arguments
 *   - result: output buffer for expanded macro
 *   - max_len: size of result buffer
 */
void expand_macro(const char *body, const char *params,
                  char args[][256], int arg_count,
                  char *result, size_t max_len);

#endif // MACRO_EXPANDER_H
