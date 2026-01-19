#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include "context.h"

/*
 * process_directive
 * -----------------
 * Detects and dispatches a preprocessor directive.
 * Returns true if the line was a directive.
 */
bool process_directive(PreprocessorContext *ctx, const char *line);

/*
 * apply_all_substitutions
 * -----------------------
 * Applies all directive-based substitutions to a line.
 * Currently handles: #define macro substitution
 * Future: Can be extended for other substitution-based directives.
 * 
 * Returns a newly allocated string. Caller must free it.
 * Returns NULL on error or if no substitutions needed (check return value).
 */
char *apply_all_substitutions(PreprocessorContext *ctx, const char *line);

typedef void (*DirectiveHandler)(PreprocessorContext *, const char *line);

typedef struct {
    const char *keyword;
    DirectiveHandler handler;
} DirectiveEntry;


#endif
