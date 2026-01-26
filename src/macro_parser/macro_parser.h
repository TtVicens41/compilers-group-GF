#ifndef MACRO_PARSER_H
#define MACRO_PARSER_H

#include <stdbool.h>
#include <stddef.h>

/*
 * is_macro_definition
 * -------------------
 * Check if definition has parentheses immediately after identifier.
 */
bool is_macro_definition(const char *after_identifier);

/*
 * parse_macro_parameters
 * ----------------------
 * Extract parameter list from macro definition.
 * Input: "(a,b)" -> Output: "a,b"
 */
bool parse_macro_parameters(const char *definition, char *params, size_t max_len);

/*
 * parse_macro_body
 * ----------------
 * Extract the body after the parameter list.
 */
bool parse_macro_body(const char *definition, char *body, size_t max_len);

/*
 * trim_whitespace
 * ---------------
 * Trim leading and trailing whitespace in place.
 */
void trim_whitespace(char *str);

#endif // MACRO_PARSER_H
