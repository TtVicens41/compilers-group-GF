#include "preprocessor/preprocessor.h"
#include "context.h"
#include "parse_arguments/parse_arguments.h"
#include "symbol_table/symbol_table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* generate_output_filename(const char* input_filename) {
    const char* dot = strrchr(input_filename, '.');
    const char* slash = strrchr(input_filename, '/');
    
    char* base = malloc(strlen(input_filename) + 10);
    if (!base) return NULL;
    
    if (dot && (!slash || dot > slash)) {
        size_t base_len = dot - input_filename;
        strncpy(base, input_filename, base_len);
        base[base_len] = '\0';
        strcat(base, "_pp");
        strcat(base, dot);
    } else {
        strcpy(base, input_filename);
        strcat(base, "_pp");
    }
    
    return base;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: No input file provided\n");
        fprintf(stderr, "Usage: %s [options] <input_file>\n", argv[0]);
        return 1;
    }
    
    PreprocessorContext ctx;
    SymbolTable symbol_table;
    
    if (argc <= 1) {
        fprintf(stdout, "No input file provided. Read -help.\n");
        return 0;
    }

    // 1. Parse arguments 
    parse_arguments(argc, argv, &ctx);

    if (ctx.help_request == true) {
        char path[128];
        sprintf(path, MANUAL_PAGE, getenv("HOME"));
        print_file(path);
        return 0;
    }

    // 2. Initialize state
    ctx.current_line = 0;
    ctx.output_enabled = true;
    ctx.in_block_comment = false;
    ctx.ifdef_depth = 0;
    ctx.ifdef_skip_depth = 0;
    
    // Initialize symbol table
    symbol_table_init(&symbol_table);
    ctx.symbol_table = &symbol_table;

    // 3. Find input filename (last non-flag argument)
    ctx.input_filename = NULL;
    for(int i = argc - 1; i >= 1; i--){
        if(argv[i][0] != '-'){
            ctx.input_filename = argv[i];
            break;
        }
    }
    
    if(!ctx.input_filename){
        fprintf(stderr, "Error: No input file specified\n");
        return 1;
    }
    
    // Generate output filename
    char* output_name = generate_output_filename(ctx.input_filename);
    if(!output_name){
        fprintf(stderr, "Error: Failed to generate output filename\n");
        return 1;
    }
    ctx.output_filename = output_name;

    // 4. Open files
    ctx.input = fopen(ctx.input_filename, "r");
    if(!ctx.input){
        fprintf(stderr, "Error: Cannot open input file '%s'\n", ctx.input_filename);
        free(output_name);
        return 1;
    }
    
    ctx.output = fopen(ctx.output_filename, "w");
    if(!ctx.output){
        fprintf(stderr, "Error: Cannot create output file '%s'\n", ctx.output_filename);
        fclose(ctx.input);
        free(output_name);
        return 1;
    }

    // 5. Run engine
    run_preprocessor(&ctx);

    // 6. Cleanup
    fclose(ctx.input);
    fclose(ctx.output);
    free(output_name);

    return 0;
}
