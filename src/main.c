#include "preprocessor.h"
#include "context.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    PreprocessorContext ctx;

    /* 1. Parse arguments (basic validation) */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    ctx.remove_comments = true;
    ctx.process_directives = true;

    /* 2. Initialize state */
    ctx.current_line = 0;
    ctx.output_enabled = true;
    ctx.in_block_comment = false;

    /* 3. Initialize define table */
    if (!define_table_init(&ctx.define_table)) {
        fprintf(stderr, "Error: Failed to initialize define table\n");
        return 1;
    }

    /* 4. Set filenames */
    ctx.input_filename = argv[1];
    ctx.output_filename = "output_pp.c";

    /* 5. Open files */
    ctx.input = fopen(ctx.input_filename, "r");
    if (ctx.input == NULL) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", ctx.input_filename);
        define_table_free(&ctx.define_table);
        return 1;
    }

    ctx.output = fopen(ctx.output_filename, "w");
    if (ctx.output == NULL) {
        fprintf(stderr, "Error: Cannot open output file '%s'\n", ctx.output_filename);
        fclose(ctx.input);
        define_table_free(&ctx.define_table);
        return 1;
    }

    /* 6. Run engine */
    run_preprocessor(&ctx);

    /* 7. Cleanup */
    fclose(ctx.input);
    fclose(ctx.output);
    define_table_free(&ctx.define_table);

    return 0;
}
