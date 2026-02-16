/**
 * @title: main.c
 * @authors: Pau Puig, Alejandro Poole, Marc Bosch, Pol Goicoechea, Davi Paiva, Joan Vicente
 * @creation: 16/02/2026
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

static void print_usage(const char *argv0) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv0);
}

int main(int argc, char *argv[]) {
    LexerContext ctx;
    Scanner *scanner = NULL;
    NFA *automaton_nfa = NULL;
    int exit_code = 0;
    memset(&ctx, 0, sizeof(ctx));

    if (argc != 2) {
        print_usage(argv[0]);
        return FILE_NOT_PROVIDED;
    }
    
    ctx.input_file = parse_arguments(argc, argv);
    if (!ctx.input_file) {
        print_usage(argv[0]);
        return FILE_NOT_PROVIDED;
    }
    ctx.input_file_str = read_file(ctx.input_file);
    if (!ctx.input_file_str) {
        exit_code = ERR_FILE_NOT_FOUND;
        goto cleanup;
    }

    ctx.categories_file = concat_strings(get_resources_dir(), CATEGORIES_FILE);
    ctx.categories_file_str = read_file(ctx.categories_file);
    if (!ctx.categories_file_str) {
        exit_code = ERR_FILE_NOT_FOUND;
        goto cleanup;
    }
    
    ctx.automata_file = concat_strings(get_resources_dir(), AUTOMATA_FILE);
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
    
    ctx.output_file_str = to_scanner_string(scanner);
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
    delete_scanner(&scanner);
    return exit_code;
}
