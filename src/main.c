#include "preprocessor/preprocessor.h"
#include "context.h"
#include "parse_arguments/parse_arguments.h"
#include "symbol_table/symbol_table.h"
#include <stdio.h>
int main(int argc, char **argv) {
    PreprocessorContext ctx;
    SymbolTable symbol_table;
    
    // 1. Parse arguments 
    parse_arguments(argc, argv, &ctx);
   

    // 2. Initialize state
    ctx.current_line = 0;
    ctx.output_enabled = true;
    ctx.in_block_comment = false;
    ctx.ifdef_depth = 0;
    ctx.ifdef_skip_depth = 0;
    
    // Initialize symbol table
    symbol_table_init(&symbol_table);
    ctx.symbol_table = &symbol_table;

    // 3. Set filenames (temporary hardcode for now)
    ctx.input_filename = argv[1];
    ctx.output_filename = "output_pp.c";

    // 4. Open files
    if(!ctx.help_request){
        ctx.input = fopen(ctx.input_filename, "r");
        ctx.output = fopen(ctx.output_filename, "w");
    }
    

    // 5. Run engine
    run_preprocessor(&ctx);

    // 6. Cleanup
    fclose(ctx.input);
    fclose(ctx.output);

    return 0;
}
