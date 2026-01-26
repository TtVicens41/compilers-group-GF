#include "directives.h"
#include "keyword_dispatcher/keyword_dispatcher.h"
#include "symbol_table/symbol_table.h"
#include "ifdef/ifdef.h"
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
    char keyword[64];
    p = extract_first_keyword(p, keyword, sizeof(keyword));
    if (!p) return;
    
    // Extract the identifier to define
    char identifier[MAX_IDENTIFIER_LEN];
    if (extract_first_keyword(p, identifier, sizeof(identifier))) {
        symbol_table_add(ctx->symbol_table, identifier);
    }
}

/*
 * handle_unsupported
 * ------------------
 * Placeholder handler for directives that are not yet implemented.
 */
static void handle_unsupported(PreprocessorContext *ctx, const char *line) {
    // For now: ignore unsupported directives, do NOT crash
    (void)ctx;   // Suppress unused parameter warning
    (void)line;  // Suppress unused parameter warning
}

/*
 * directive_table
 * ---------------
 * Mapping of directive keywords to their handler functions.
 * Terminated by {NULL, NULL} sentinel.
 */
static KeywordHandlerPair directive_table[] = {
    {"define", handle_define},
    {"include", handle_unsupported},
    {"ifdef", handle_ifdef},
    {"endif", handle_endif},
    {NULL, NULL}
};

/*
 * process_directive
 * -----------------
 * Detects and dispatches preprocessor directives.
 * Returns true if the line was a directive (consumed).
 */
bool process_directive(PreprocessorContext *ctx, const char *line) {
    const char *p = line;

    // Skip leading whitespace
    while (isspace(*p)) p++;
    
    // Check if line starts with '#'
    if (*p != '#') return false;

    // Skip the '#' 
    p++;

    // Extract the directive keyword (extract_first_keyword handles whitespace)
    char keyword[64];
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
