/**
 * @file main.c
 * @brief Main Program for the Lexical Analyzer
 * 
 * The following steps of the main are:
 * 
 * 1) Parse arguments (call Man Page if needed)
 * 2) Read input file to compile
 * 3) Read automata configuration file
 * 4) Initialize automata data structures
 * 5) Call scanner
 * 6) Write scan to output file.
 * 7) Cleanup.
 *  
 * @authors Marc Bosch Manzano, Joan Vicente, Pau Puig, Alejandro Poole,  
 * Davi Paiva, & Pol Goicoechea.
 * @since 2026-01-30
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./context.h"
#include "./parse_arguments/parse_arguments.h"
#include "./language_defs.h"
#include "./utils/utils.h"
#include "./automata/automata.h"
#include "./scanner/scanner.h"

int main(int argc, char *argv[]) {
    LexerContext ctx = { 0 };
    TokenLines *scanner = NULL;
    NFA *automaton_nfa = NULL;
    int exit_code = 0;

    if (argc <= 1) {
        print_file(resolve_resource_path(argv[0], MANUAL_PAGE));
        return FILE_NOT_PROVIDED;
    }
    
    ctx.input_file = parse_arguments(argc, argv);
    if (!ctx.input_file) {
        return FILE_NOT_PROVIDED;
    }

    ctx.input_file_str = read_file(ctx.input_file);
    if (!ctx.input_file_str) {
        exit_code = ERR_FILE_NOT_FOUND;
        goto cleanup;
    }

    ctx.categories_file = resolve_resource_path(argv[0], CATEGORIES_FILE);
    ctx.categories_file_str = read_file(ctx.categories_file);
    if (!ctx.categories_file_str) {
        exit_code = ERR_FILE_NOT_FOUND;
        goto cleanup;
    }
    
    ctx.automata_file = resolve_resource_path(argv[0], AUTOMATA_FILE);
    ctx.automata_file_str = read_file(ctx.automata_file);
    if (!ctx.automata_file_str) {
        exit_code = ERR_FILE_NOT_FOUND;
        goto cleanup;
    }

    automaton_nfa = init_union_nfa(ctx.automata_file_str);
    if (!is_valid_nfa(automaton_nfa, ctx.categories_file_str)) {
        exit_code = ERR_AUTOMATA_NOT_LOADED;
        goto cleanup;
    }

    scanner = scan_string(automaton_nfa, ctx.input_file_str);
    if (!scanner) {
        exit_code = ERR_AUTOMATA_NOT_LOADED;
        goto cleanup;
    }
    
    ctx.output_file_str = to_token_lines_string(scanner);
    if (!ctx.output_file_str) {
        exit_code = ERR_AUTOMATA_NOT_LOADED;
        goto cleanup;
    }

    ctx.output_file = concat_strings(ctx.input_file, SCANNER_OUTPUT_SUFFIX);
    if (!ctx.output_file) {
        exit_code = ERR_FILE_NOT_FOUND;
        goto cleanup;
    }

    write_file(ctx.output_file, ctx.output_file_str);
    
    printf("Input file: %s\n", ctx.input_file);
    printf("Output file: %s\n", ctx.output_file);
    printf("Lexical analysis done!\n");

cleanup:
    clear_context(&ctx);
    delete_nfa(&automaton_nfa);
    delete_token_lines(&scanner);
    return exit_code;
}
