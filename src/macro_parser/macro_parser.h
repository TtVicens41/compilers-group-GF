/**
 * @title: Macro Parser.
 * @brief: Declarations of functions to process the macros. Most of functions 
 * process strings, the definition of a macro, and return a boolean.
 * @authors: Davi Panna-Mattos Dias de Paiva, Marc Bosch Manzano.
 * @creation: before 2026/01/27.
 */

#ifndef MACRO_PARSER_H
#define MACRO_PARSER_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Check if definition has a parenthesis immediately after identifier.
 * @param after_identifier An string to be checked.
 * @return True if parenthesis is found.
 */
bool is_macro_definition(const char *after_identifier);

/**
 * Extract parameter list from macro definition. For example, with
 * "(a,b)" gets an output "a,b".
 * @param definition The definition of a macro.
 * @param params The parameters of a macro as a string.
 * @param max_len The max string length between "(" and ")" of the definition 
 * of the macro.
 * @return True if the parsing was done correctly.
 */
bool parse_macro_parameters(const char *definition, char *params, size_t max_len);

/**
 * Extract the body after the parameter list.
 * @param definition The definition of a macro.
 * @param body The body of a macro.
 * @param max_len The max string length between "(" and ")" of the body 
 * of the macro.
 */
bool parse_macro_body(const char *definition, char *body, size_t max_len);


#endif // MACRO_PARSER_H
