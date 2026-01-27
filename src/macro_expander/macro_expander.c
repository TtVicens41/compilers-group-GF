/**
 * @title: Macro Expander.
 * @brief: Implementations of functions to process the macros. 
 * @authors: Davi Panna-Mattos Dias de Paiva¡.
 * @creation: before 2026/01/27.
 */

#include <string.h>
#include <ctype.h>

#include "macro_expander.h"
#include "macro_parser/macro_parser.h"
#include "../utils/string_utils.h"
#include "../language_defs.h"

const char *find_matching_paren(const char *str) {
    int depth = 0;
    while (*str) {
        if (*str == CHAR_PAREN_OPEN) depth++;
        else if (*str == CHAR_PAREN_CLOSE) {
            depth--;
            if (depth == 0) return str;
        }
        str++;
    }
    return NULL;
}

int parse_macro_arguments(const char *call, char args[][MAX_ARG_LENGTH], int max_args) {
    const char *start = strchr(call, CHAR_PAREN_OPEN);
    if (!start) return 0;
    start++; // Skip opening parenthesis
    
    const char *end = find_matching_paren(call);
    if (!end) return 0;
    
    int arg_count = 0;
    const char *arg_start = start;
    int paren_depth = 0;
    
    for (const char *p = start; p < end; p++) {
        if (*p == CHAR_PAREN_OPEN) 
            paren_depth++;
        else if (*p == CHAR_PAREN_CLOSE) 
            paren_depth--;
        else if (*p == CHAR_COMMA && paren_depth == 0) {
            // End of argument
            size_t len = p - arg_start;
            if (len >= MAX_ARG_LENGTH) len = MAX_ARG_LENGTH - 1;
            strncpy(args[arg_count], arg_start, len);
            args[arg_count][len] = '\0';
            trim_whitespace(args[arg_count]);
            arg_count++;
            arg_start = p + 1;
            
            if (arg_count >= max_args) break;
        }
    }
    
    // Last argument
    if (arg_count < max_args && arg_start < end) {
        size_t len = end - arg_start;
        if (len >= MAX_ARG_LENGTH) len = MAX_ARG_LENGTH - 1;
        strncpy(args[arg_count], arg_start, len);
        args[arg_count][len] = '\0';
        trim_whitespace(args[arg_count]);
        arg_count++;
    }
    
    return arg_count;
}

static void parse_parameter_names(const char *params, char param_names[][MAX_IDENTIFIER_LENGTH], int *count) {
    char params_copy[MAX_ARG_LENGTH];
    strncpy(params_copy, params, MAX_ARG_LENGTH - 1);
    params_copy[MAX_ARG_LENGTH - 1] = '\0';
    
    *count = 0;
    char *token = strtok(params_copy, ",");
    while (token && *count < MAX_MACRO_ARGS) {
        strncpy(param_names[*count], token, MAX_IDENTIFIER_LENGTH - 1);
        param_names[*count][MAX_IDENTIFIER_LENGTH - 1] = '\0';
        trim_whitespace(param_names[*count]);
        (*count)++;
        token = strtok(NULL, ",");
    }
}

static bool replace_parameter(const char *identifier, char param_names[][MAX_IDENTIFIER_LENGTH], int param_count,
                              char args[][MAX_ARG_LENGTH], int arg_count, char **dst, size_t *remaining) {
    for (int p = 0; p < param_count && p < arg_count; p++) {
        if (strcmp(identifier, param_names[p]) == 0) {
            size_t arg_len = strlen(args[p]);
            if (arg_len > *remaining) arg_len = *remaining;
            strncpy(*dst, args[p], arg_len);
            *dst += arg_len;
            *remaining -= arg_len;
            return true;
        }
    }
    return false;
}

void expand_macro(const char *body, const char *params,
                  char args[][MAX_ARG_LENGTH], int arg_count,
                  char *result, size_t max_len) {
    // Parse parameter names
    char param_names[MAX_MACRO_ARGS][MAX_IDENTIFIER_LENGTH];
    int param_count;
    parse_parameter_names(params, param_names, &param_count);
    
    // Perform substitution
    const char *src = body;
    char *dst = result;
    size_t remaining = max_len - 1;
    
    while (*src && remaining > 0) {
        if (IS_IDENTIFIER_START(*src)) {
            // Extract identifier
            char identifier[MAX_IDENTIFIER_LENGTH];
            int i = 0;
            while (IS_IDENTIFIER_CHAR(*src) && i < MAX_IDENTIFIER_LENGTH - 1) {
                identifier[i++] = *src++;
            }
            identifier[i] = '\0';
            
            // Try to replace with argument
            if (!replace_parameter(identifier, param_names, param_count, args, arg_count, &dst, &remaining)) {
                // Keep original identifier
                size_t id_len = strlen(identifier);
                if (id_len > remaining) id_len = remaining;
                strncpy(dst, identifier, id_len);
                dst += id_len;
                remaining -= id_len;
            }
        } else {
            *dst++ = *src++;
            remaining--;
        }
    }
    *dst = '\0';
}
