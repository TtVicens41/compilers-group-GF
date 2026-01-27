/**
 * @title: Run Preprocessor.
 * @brief: Implements the main function to run the preprocessor
 * using the context data structure.
 * @author: Davi Panna-Mattos Dias de Paiva
 * @creation: before 2026/01/27
 */


#include <string.h>

#include "preprocessor.h"
#include "directives/directives.h"
#include "comments/comments.h"
#include "define/define.h"

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
        if (ctx->process_directives && process_directive(ctx, line)) {
            continue;
        }
        
        // Apply #define substitutions only if directives are enabled
        if (ctx->process_directives) {
            replace_defines_in_line(ctx, line, processed_line);
        } else {
            strcpy(processed_line, line);
        }

        // Output the processed line if output is enabled
        if (ctx->output_enabled) {
            fputs(processed_line, ctx->output);
        }
    }
}
