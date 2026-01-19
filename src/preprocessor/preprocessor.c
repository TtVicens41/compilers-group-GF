#include "preprocessor.h"
#include "directives.h"
#include "comments.h"
#include <stdlib.h>
#include <string.h>

/*
 * run_preprocessor
 * ----------------
 * Main preprocessing loop. Processes input line by line:
 * 1. Remove comments (if enabled)
 * 2. Detect and handle directives
 * 3. Apply macro substitutions (via generic directives function)
 * 4. Write result to output
 */
void run_preprocessor(PreprocessorContext *ctx) {
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), ctx->input)) {
        ctx->current_line++;

        /* Step 1: Remove comments if enabled */
        if (ctx->remove_comments) {
            remove_comments(line, &ctx->in_block_comment);
        }

        /* Step 2: Detect and process directives */
        if (ctx->process_directives && process_directive(ctx, line)) {
            /* Directive was handled, do not write to output */
            continue;
        }

        /* Step 3: Apply all directive-based substitutions (e.g., #define) */
        if (ctx->process_directives && ctx->define_table.count > 0) {
            char *substituted = apply_all_substitutions(ctx, line);
            if (substituted != NULL) {
                /* Write substituted line to output */
                if (ctx->output_enabled) {
                    fputs(substituted, ctx->output);
                }
                free(substituted);
                continue;
            }
            /* If substitution failed, fall through to write original line */
        }

        /* Step 4: Write line to output if enabled */
        if (ctx->output_enabled) {
            fputs(line, ctx->output);
        }
    }
}
