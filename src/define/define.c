#include "define.h"
#include "symbol_table/symbol_table.h"
#include "keyword_dispatcher/keyword_dispatcher.h"
#include <string.h>
#include <ctype.h>

/*
 * handle_define
 * -------------
 * Parses #define directive and adds identifier to symbol table.
 * Syntax: #define <identifier>
 */
void handle_define(PreprocessorContext *ctx, const char *line) {
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
