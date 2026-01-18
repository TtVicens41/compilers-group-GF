#include "preprocessor.h"
#include "directives.h"
#include "comments.h"
#include <string.h>

void run_preprocessor(PreprocessorContext *ctx) {
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), ctx->input)) {
        ctx->current_line++;

        // TODO: remove comments
        if (ctx->remove_comments) {
            remove_comments(line, &ctx->in_block_comment);
        }
        // TODO: detect directive
        if (process_directive(ctx, line)) {
            continue;
        }
        // TODO: dispatch directive
        // TODO: apply substitutions

        if (ctx->output_enabled) {
            fputs(line, ctx->output);
        }
    }
}
