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

typedef void (*DirectiveHandler)(PreprocessorContext *, const char *line);

typedef struct {
    const char *keyword;
    DirectiveHandler handler;
} DirectiveEntry;


#endif
