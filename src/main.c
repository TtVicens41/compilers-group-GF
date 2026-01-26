#include "preprocessor.h"
#include "context.h"
#include "parse_arguments.h"
int main(int argc, char **argv) {
    PreprocessorContext ctx;
    
    parse_arguments(argc, argv, &ctx);
    // 1. Parse arguments (later)
    
    ctx.remove_comments = true;
    ctx.process_directives = true;

    // 2. Initialize state
    ctx.current_line = 0;
    ctx.output_enabled = true;
    ctx.in_block_comment = false;

    // 3. Set filenames (temporary hardcode for now)
    ctx.input_filename = argv[1];
    ctx.output_filename = "output_pp.c";

    // 4. Open files
    ctx.input = fopen(ctx.input_filename, "r");
    ctx.output = fopen(ctx.output_filename, "w");

    // 5. Run engine
    run_preprocessor(&ctx);

    // 6. Cleanup
    fclose(ctx.input);
    fclose(ctx.output);

    return 0;
}
