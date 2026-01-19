#include "directives.h"
#include "define/define.h"
#include <string.h>
#include <ctype.h>

/*
 * Handler for unsupported directives.
 * Does nothing - directive line is simply not written to output.
 */
static void handle_unsupported(PreprocessorContext *ctx, const char *line) {
    (void)ctx;
    (void)line;
    /* For now: ignore unsupported directives, do NOT crash */
}

/*
 * Handler for #define directive.
 * Parses the directive and stores the definition in the context's define table.
 */
static void handle_define_directive(PreprocessorContext *ctx, const char *line) {
    handle_define(&ctx->define_table, line, ctx->current_line);
}

/* Dispatch table mapping directive keywords to their handlers */
static DirectiveEntry directive_table[] = {
    {"define", handle_define_directive},
    {"include", handle_unsupported},
    {"ifdef", handle_unsupported},
    {"endif", handle_unsupported},
    {NULL, NULL}
};

/*
 * Detects and dispatches preprocessor directives.
 * Returns true if the line was a directive (should not be written to output).
 */
bool process_directive(PreprocessorContext *ctx, const char *line) {
    const char *p = line;

    while (isspace((unsigned char)*p)) p++;
    if (*p != '#') return false;

    p++; /* skip '#' */
    while (isspace((unsigned char)*p)) p++;

    char keyword[32];
    int i = 0;
    while (*p && !isspace((unsigned char)*p) && i < 31) {
        keyword[i++] = *p++;
    }
    keyword[i] = '\0';

    for (int j = 0; directive_table[j].keyword; j++) {
        if (strcmp(keyword, directive_table[j].keyword) == 0) {
            directive_table[j].handler(ctx, line);
            return true;
        }
    }

    handle_unsupported(ctx, line);
    return true;
}

/*
 * apply_all_substitutions
 * -----------------------
 * Generic function to apply all directive-based substitutions.
 * Centralizes substitution logic so preprocessor.c doesn't need to
 * know about specific directive modules.
 * 
 * Currently handles:
 *   - #define macro substitution
 * 
 * Future extensions:
 *   - Other substitution-based directives can be added here
 */
char *apply_all_substitutions(PreprocessorContext *ctx, const char *line) {
    if (ctx == NULL || line == NULL) return NULL;
    
    /* Apply #define macro substitutions */
    return apply_substitutions(&ctx->define_table, line);
}
