#include "ifdef.h"
#include "symbol_table/symbol_table.h"
#include "keyword_dispatcher/keyword_dispatcher.h"
#include <string.h>
#include <ctype.h>

/*
 * handle_ifdef
 * ------------
 * Checks if identifier is defined and toggles output accordingly.
 * Syntax: #ifdef <identifier>
 */
void handle_ifdef(PreprocessorContext *ctx, const char *line) {
    if (!ctx || !ctx->symbol_table) return;
    
    // Increment nesting depth
    ctx->ifdef_depth++;
    
    // If we're already skipping, continue skipping nested ifdefs
    if (ctx->ifdef_skip_depth > 0) {
        return;
    }
    
    // Find the start of the directive content (after #ifdef)
    const char *p = line;
    while (*p && *p != '#') p++;
    if (*p == '#') p++;
    
    // Skip "ifdef" keyword
    char keyword[64];
    p = extract_first_keyword(p, keyword, sizeof(keyword));
    if (!p) {
        // No keyword after #ifdef, treat as undefined
        ctx->output_enabled = false;
        ctx->ifdef_skip_depth = ctx->ifdef_depth;
        return;
    }
    
    // Extract the identifier to check
    char identifier[MAX_IDENTIFIER_LEN];
    if (extract_first_keyword(p, identifier, sizeof(identifier))) {
        // Check if identifier is defined in the symbol table
        if (!symbol_table_contains(ctx->symbol_table, identifier)) {
            // Not defined, disable output and mark skip depth
            ctx->output_enabled = false;
            ctx->ifdef_skip_depth = ctx->ifdef_depth;
        }
        // If defined, keep output enabled (do nothing)
    } else {
        // No identifier found, treat as undefined
        ctx->output_enabled = false;
        ctx->ifdef_skip_depth = ctx->ifdef_depth;
    }
}

/*
 * handle_endif
 * ------------
 * Closes an ifdef block and restores output state if necessary.
 */
void handle_endif(PreprocessorContext *ctx, const char *line) {
    if (!ctx) return;
    
    // If we're exiting the level where we started skipping, re-enable output
    if (ctx->ifdef_skip_depth == ctx->ifdef_depth) {
        ctx->output_enabled = true;
        ctx->ifdef_skip_depth = 0;
    }
    
    // Decrement nesting depth
    if (ctx->ifdef_depth > 0) {
        ctx->ifdef_depth--;
    }
}