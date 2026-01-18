#include "preprocessor.h"
#include "directives.h"
#include <string.h>

#define MAX_LINE 1024

void run_preprocessor(PreprocessorContext *ctx) {
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), ctx->input)) {
        ctx->current_line++;

        // TODO: remove comments
        // TODO: detect directive
        // TODO: dispatch directive
        // TODO: apply substitutions

        if (ctx->output_enabled) {
            fputs(line, ctx->output);
        }
    }
}
