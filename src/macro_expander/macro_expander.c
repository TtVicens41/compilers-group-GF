#include "macro_expander.h"
#include "macro_parser/macro_parser.h"
#include <string.h>
#include <ctype.h>

const char *find_matching_paren(const char *str) {
    int depth = 0;
    while (*str) {
        if (*str == '(') depth++;
        else if (*str == ')') {
            depth--;
            if (depth == 0) return str;
        }
        str++;
    }
    return NULL;
}

int parse_macro_arguments(const char *call, char args[][256], int max_args) {
    const char *start = strchr(call, '(');
    if (!start) return 0;
    start++; // Skip '('
    
    const char *end = find_matching_paren(call);
    if (!end) return 0;
    
    int arg_count = 0;
    const char *arg_start = start;
    int paren_depth = 0;
    
    for (const char *p = start; p < end; p++) {
        if (*p == '(') paren_depth++;
        else if (*p == ')') paren_depth--;
        else if (*p == ',' && paren_depth == 0) {
            // End of argument
            size_t len = p - arg_start;
            if (len >= 256) len = 255;
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
        if (len >= 256) len = 255;
        strncpy(args[arg_count], arg_start, len);
        args[arg_count][len] = '\0';
        trim_whitespace(args[arg_count]);
        arg_count++;
    }
    
    return arg_count;
}

static void parse_parameter_names(const char *params, char param_names[][64], int *count) {
    char params_copy[256];
    strncpy(params_copy, params, 255);
    params_copy[255] = '\0';
    
    *count = 0;
    char *token = strtok(params_copy, ",");
    while (token && *count < 10) {
        strncpy(param_names[*count], token, 63);
        param_names[*count][63] = '\0';
        trim_whitespace(param_names[*count]);
        (*count)++;
        token = strtok(NULL, ",");
    }
}

static bool replace_parameter(const char *identifier, char param_names[][64], int param_count,
                              char args[][256], int arg_count, char **dst, size_t *remaining) {
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
                  char args[][256], int arg_count,
                  char *result, size_t max_len) {
    // Parse parameter names
    char param_names[10][64];
    int param_count;
    parse_parameter_names(params, param_names, &param_count);
    
    // Perform substitution
    const char *src = body;
    char *dst = result;
    size_t remaining = max_len - 1;
    
    while (*src && remaining > 0) {
        if (isalpha(*src) || *src == '_') {
            // Extract identifier
            char identifier[64];
            int i = 0;
            while ((isalnum(*src) || *src == '_') && i < 63) {
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
