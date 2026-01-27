#include "directives.h"
#include "keyword_dispatcher/keyword_dispatcher.h"
#include "define/define.h"
#include "ifdef/ifdef.h"
#include "include/include.h"
#include <string.h>
#include <ctype.h>

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
    {"include", handle_include},
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
