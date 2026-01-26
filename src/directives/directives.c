#include "directives.h"
#include "keyword_dispatcher.h"
#include "symbol_table/symbol_table.h"
#include <string.h>
#include <ctype.h>

/*
 * handle_define
 * -------------
 * Parses #define directive and adds identifier to symbol table.
 * Syntax: #define <identifier>
 */
static void handle_define(PreprocessorContext *ctx, const char *line) {
    if (!ctx || !ctx->symbol_table) return;
    
    // Find the start of the directive content (after #define)
    const char *p = line;
    while (*p && *p != '#') p++;
    if (*p == '#') p++;
    
    // Skip "define" keyword
    char keyword[32];
    p = extract_first_keyword(p, keyword, sizeof(keyword));
    if (!p) return;
    
    // Extract the identifier to define
    char identifier[MAX_IDENTIFIER_LEN];
    if (extract_first_keyword(p, identifier, sizeof(identifier))) {
        symbol_table_add(ctx->symbol_table, identifier);
    }
}

/*
 * handle_ifdef
 * ------------
 * Checks if identifier is defined and toggles output accordingly.
 * Syntax: #ifdef <identifier>
 */
static void handle_ifdef(PreprocessorContext *ctx, const char *line) {
    if (!ctx || !ctx->symbol_table) return;
    
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
    char keyword[32];
    p = extract_first_keyword(p, keyword, sizeof(keyword));
    if (!p) return;
    
    // Extract the identifier to check
    char identifier[MAX_IDENTIFIER_LEN];
    if (extract_first_keyword(p, identifier, sizeof(identifier))) {
        // Check if identifier is defined
        if (!symbol_table_contains(ctx->symbol_table, identifier)) {
            // Not defined, disable output and mark skip depth
            ctx->output_enabled = false;
            ctx->ifdef_skip_depth = ctx->ifdef_depth;
        }
        // If defined, keep output enabled (do nothing)
    }
}

/*
 * handle_endif
 * ------------
 * Closes an ifdef block and restores output state if necessary.
 */
static void handle_endif(PreprocessorContext *ctx, const char *line) {
    if (!ctx) return;
    
    // If we're exiting the level where we started skipping, re-enable output
    if (ctx->ifdef_skip_depth == ctx->ifdef_depth) {
        ctx->output_enabled = true;
        ctx->ifdef_skip_depth = 0;
    }
    
    if (ctx->ifdef_depth > 0) {
        ctx->ifdef_depth--;
    }
}

static void handle_unsupported(PreprocessorContext *ctx, const char *line) {
    // For now: copy directive as-is or ignore, but do NOT crash
}

static KeywordHandlerPair directive_table[] = {
    {"define", handle_define},
    {"include", handle_unsupported},
    {"ifdef", handle_ifdef},
    {"endif", handle_endif},
    {NULL, NULL}
};

bool process_directive(PreprocessorContext *ctx, const char *line) {
    const char *p = line;

    // Skip leading whitespace
    while (isspace(*p)) p++;
    
    // Check if line starts with '#'
    if (*p != '#') return false;

    // Skip the '#' 
    p++;

    // Extract the directive keyword (extract_first_keyword handles whitespace)
    char keyword[32];
    if (!extract_first_keyword(p, keyword, sizeof(keyword))) {
        // No keyword found after '#'
        handle_unsupported(ctx, line);
        return true;
    }

    // Dispatch to the appropriate handler
    if (!find_and_handle_keyword(ctx, keyword, directive_table, line)) {
        // Keyword not found in table, handle as unsupported
        handle_unsupported(ctx, line);
    }

    return true;
}
