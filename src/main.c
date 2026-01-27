/**
 * @title: Main File for Preprocessor.
 * @authors: Davi Panna-Mattos Dias de Paiva, Pau Puig Guillén, Alejandro 
 * Poole Becerra, Marc Bosch Manzano, Joan Vicente Martín.
 * @creation: before 2026/01/27.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "preprocessor/preprocessor.h"
#include "context.h"
#include "parse_arguments/parse_arguments.h"
#include "symbol_table/symbol_table.h"
#include "utils/string_utils.h"
#include "utils/error_utils.h"
#include "utils/file_utils.h"

#define PREPROCESSOR_EXTENSION "_pp"

int main(int argc, char **argv) {
    PreprocessorContext ctx;
    SymbolTable symbol_table;

    // 1. Parse arguments 
    parse_arguments(argc, argv, &ctx);

    // 2. Skip: print manual page
    if(ctx.help_request == true) {
        char path[128];
        sprintf(path, MANUAL_PAGE, getenv("HOME"));
        print_file(path);
        return 0;
    }

    // 3. Skip: file not provided
    if(!ctx.input_filename){
        fprintf(stderr, "Error: No input file specified\n");
        fprintf(stderr, "Usage: %s [options] <input_file>\n", argv[0]);
        return 1;
    }

    // 4. Initialize symbol table
    symbol_table_init(&symbol_table);
    ctx.symbol_table = &symbol_table;
    
    // 5. Generate output filename
    char* output_name = add_suffix_to_path(ctx.input_filename, PREPROCESSOR_EXTENSION);
    if(!output_name){
        fprintf(stderr, "Error: Failed to generate output filename\n");
        return 1;
    }
    ctx.output_filename = output_name;

    // 6. Open input file
    ctx.input = fopen(ctx.input_filename, "r");
    if(!ctx.input){
        fprintf(stderr, "Error: Cannot open input file '%s'\n", ctx.input_filename);
        free(output_name);
        return 1;
    }
    
    // 7. Open output file
    ctx.output = fopen(ctx.output_filename, "w");
    if(!ctx.output){
        
        fprintf(stderr, "Error: Cannot create output file '%s'\n", ctx.output_filename);
        fclose(ctx.input);
        free(output_name);
        return 1;
    }

    // 8. Run engine
    run_preprocessor(&ctx);

    // 9. Cleanup
    fclose(ctx.input);
    fclose(ctx.output);
    free(output_name);

    return 0;
}
