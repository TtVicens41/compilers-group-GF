#include "preprocessor.h"
#include "directives/directives.h"
#include "comments/comments.h"
#include "define/define.h"
#include <string.h>

void run_preprocessor(PreprocessorContext *ctx) {
    char line[MAX_LINE];
    char processed_line[MAX_LINE];

    while (fgets(line, sizeof(line), ctx->input)) {
        ctx->current_line++;

        // Remove comments if enabled
        if (ctx->remove_comments) {
            remove_comments(line, &ctx->in_block_comment);
        }
        
        // Process directives (returns true if line was a directive)
        if (process_directive(ctx, line)) {
            continue;
        }
        
        // Apply #define substitutions
        replace_defines_in_line(ctx, line, processed_line);

        // Output the processed line if output is enabled
        if (ctx->output_enabled) {
            fputs(processed_line, ctx->output);
        }
    }
}
