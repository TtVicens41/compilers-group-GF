/**
 * @title: Macro Parser.
 * @brief: Implementations of functions to process the macros. 
 * @authors: Davi Panna-Mattos Dias de Paiva, Marc Bosch Manzano.
 * @creation: before 2026/01/27.
 */

#include <string.h>
#include <ctype.h>

#include "macro_parser.h"
#include "../utils/string_utils.h"

bool is_macro_definition(const char *after_identifier) {
    while (isspace(*after_identifier)) after_identifier++;
    return *after_identifier == '(';
}

bool parse_macro_parameters(const char *definition, char *params, size_t max_len) {
    const char *start = strchr(definition, '(');
    if (!start) 
        return false;
    start++; // Skip '('
    
    const char *end = strchr(start, ')');
    if (!end) 
        return false;
    
    size_t len = end - start;
    if (len >= max_len) 
        len = max_len - 1;
    
    strncpy(params, start, len);
    params[len] = '\0';
    trim_whitespace(params);
    
    return true;
}

bool parse_macro_body(const char *definition, char *body, size_t max_len) {
    const char *end_paren = strchr(definition, ')');
    if (!end_paren) 
        return false;
    
    const char *body_start = end_paren + 1;
    while (isspace(*body_start)) 
        body_start++;
    
    strncpy(body, body_start, max_len - 1);
    body[max_len - 1] = '\0';
    trim_whitespace(body);
    
    return true;
}
