/**
 * @title: Define Directive Processing.
 * @authors: Pau Puig Guillén
 * @creation: before 2026/01/27
 */

#include "define.h"
#include "symbol_table/symbol_table.h"
#include "keyword_dispatcher/keyword_dispatcher.h"
#include "macro_parser/macro_parser.h"
#include "macro_expander/macro_expander.h"
#include <string.h>
#include <ctype.h>

/*
 * handle_simple_define
 * --------------------
 * Handle a simple constant definition.
 */
static void handle_simple_define(PreprocessorContext *ctx, const char *identifier, const char *p) {
    // Skip leading whitespace
    while (isspace(*p)) p++;
    
    // Trim trailing whitespace
    const char *value_end = p + strlen(p) - 1;
    while (value_end > p && isspace(*value_end)) value_end--;
    
    // Copy value into buffer
    char value[MAX_VALUE_LEN];
    size_t value_len = value_end - p + 1;
    if (value_len >= MAX_VALUE_LEN) value_len = MAX_VALUE_LEN - 1;
    strncpy(value, p, value_len);
    value[value_len] = '\0';
    
    symbol_table_add_with_value(ctx->symbol_table, identifier, value);
}

/*
 * handle_macro_define
 * -------------------
 * Handle a macro definition with parameters.
 */
static void handle_macro_define(PreprocessorContext *ctx, const char *identifier, const char *after_id) {
    char params[MAX_VALUE_LEN];
    char body[MAX_VALUE_LEN];
    
    if (parse_macro_parameters(after_id, params, sizeof(params)) &&
        parse_macro_body(after_id, body, sizeof(body))) {
        symbol_table_add_macro(ctx->symbol_table, identifier, params, body);
    }
}

/*
 * handle_define
 * -------------
 * Main handler for #define directive.
 * Detects and delegates to constant or macro handler.
 */
void handle_define(PreprocessorContext *ctx, const char *line) {
    if (!ctx || !ctx->symbol_table) return;
    
    // Skip to content after '#define'
    const char *p = line;
    while (isspace(*p)) p++;
    if (*p == '#') p++;
    
    char keyword[64];
    p = extract_first_keyword(p, keyword, sizeof(keyword));
    if (!p) return;
    
    // Extract identifier
    char identifier[MAX_IDENTIFIER_LEN];
    const char *after_id = p;
    p = extract_first_keyword(p, identifier, sizeof(identifier));
    if (!p) return;
    
    // Check if it's a macro or simple constant
    if (is_macro_definition(after_id + strlen(identifier))) {
        handle_macro_define(ctx, identifier, after_id);
    } else {
        handle_simple_define(ctx, identifier, p);
    }
}

/*
 * replace_simple_define
 * ---------------------
 * Replace a simple constant with its value.
 */
static void replace_simple_define(const char *identifier, const char *value, 
                                  char **dst, size_t *remaining) {
    size_t val_len = strlen(value);
    if (val_len > *remaining) val_len = *remaining;
    strncpy(*dst, value, val_len);
    *dst += val_len;
    *remaining -= val_len;
}

/*
 * replace_macro_call
 * ------------------
 * Expand and replace a macro call.
 */
static bool replace_macro_call(PreprocessorContext *ctx, const char *identifier,
                               const char **src, char **dst, size_t *remaining) {
    // Check if followed by '(' (macro call)
    const char *temp = *src;
    while (isspace(*temp)) temp++;
    
    if (*temp != '(') return false;
    
    // Parse macro call arguments
    char args[MAX_MACRO_ARGS][MAX_VALUE_LEN];
    int arg_count = parse_macro_arguments(*src, args, MAX_MACRO_ARGS);
    
    // Get macro definition
    const char *params = symbol_table_get_parameters(ctx->symbol_table, identifier);
    const char *body = symbol_table_get_value(ctx->symbol_table, identifier);
    
    if (!params || !body) return false;
    
    // Expand macro
    char expanded[MAX_VALUE_LEN];
    expand_macro(body, params, args, arg_count, expanded, sizeof(expanded));
    
    size_t exp_len = strlen(expanded);
    if (exp_len > *remaining) exp_len = *remaining;
    strncpy(*dst, expanded, exp_len);
    *dst += exp_len;
    *remaining -= exp_len;
    
    // Skip past the macro call
    const char *end_paren = find_matching_paren(*src);
    if (end_paren) {
        *src = end_paren + 1;
    }
    
    return true;
}

/*
 * replace_identifier
 * ------------------
 * Replace an identifier with its defined value or expand if it's a macro.
 */
static void replace_identifier(PreprocessorContext *ctx, const char *identifier,
                               const char **src, char **dst, size_t *remaining) {
    // Check if it's a macro
    if (symbol_table_is_macro(ctx->symbol_table, identifier)) {
        if (replace_macro_call(ctx, identifier, src, dst, remaining)) {
            return;
        }
    }
    
    // Check if it's a simple define
    const char *value = symbol_table_get_value(ctx->symbol_table, identifier);
    if (value && *value) {
        replace_simple_define(identifier, value, dst, remaining);
    } else {
        // Keep original identifier
        size_t id_len = strlen(identifier);
        if (id_len > *remaining) id_len = *remaining;
        strncpy(*dst, identifier, id_len);
        *dst += id_len;
        *remaining -= id_len;
    }
}

/*
 * replace_defines_in_line
 * -----------------------
 * Main function to replace all defines and expand macros in a line.
 */
void replace_defines_in_line(PreprocessorContext *ctx, const char *line, char *output_line) {
    if (!ctx || !ctx->symbol_table || !line || !output_line) {
        if (output_line && line) strcpy(output_line, line);
        return;
    }
    
    const char *src = line;
    char *dst = output_line;
    size_t remaining = MAX_VALUE_LEN - 1;

    while (*src && remaining > 0) {
        if (isalpha(*src) || *src == '_') {
            // Extract identifier
            char identifier[MAX_IDENTIFIER_LEN];
            int i = 0;
            
            while ((isalnum(*src) || *src == '_') && i < MAX_IDENTIFIER_LEN - 1) {
                identifier[i++] = *src++;
            }
            identifier[i] = '\0';
            
            // Replace identifier
            replace_identifier(ctx, identifier, &src, &dst, &remaining);
        } else {
            *dst++ = *src++;
            remaining--;
        }
    }
    *dst = '\0';
}
