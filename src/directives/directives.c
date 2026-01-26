#include "directives.h"
#include "keyword_dispatcher.h"
#include <string.h>
#include <ctype.h>

static void handle_unsupported(PreprocessorContext *ctx, const char *line) {
    // For now: copy directive as-is or ignore, but do NOT crash
}

static KeywordHandlerPair directive_table[] = {
    {"define", handle_unsupported},
    {"include", handle_unsupported},
    {"ifdef", handle_unsupported},
    {"endif", handle_unsupported},
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
