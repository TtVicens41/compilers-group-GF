/**
 * Main Program for Lexer/Scanner
 * 
 * @steps:
 *      1) Parse arguments (cal Man Page if needed)
 *      2) Read input file to compile
 *      3) Read automata configuration file
 *      4) Initialize automata data structures
 *      5) Call scanner
 *      6) Write scan to output file.
 *      7) Cleanup.
 *  
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
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

static int COUNT = 0;

int main(int argc, char *argv[]) {
    LexerContext ctx;
    Scanner *scanner;
    NFA *automaton_nfa;
    
    ctx.input_file = parse_arguments(argc, argv);
    ctx.input_file_str = read_file(ctx.input_file);
    if (!ctx.input_file_str) {
        return ERR_FILE_NOT_FOUND;
    }

    ctx.categories_file = concat_strings(get_resources_dir(), CATEGORIES_FILE);
    ctx.categories_file_str = read_file(ctx.categories_file);
    if (!ctx.categories_file_str) {
        return ERR_FILE_NOT_FOUND;
    }
    
    ctx.automata_file = concat_strings(get_resources_dir(), AUTOMATA_FILE);
    ctx.automata_file_str = read_file(ctx.automata_file);
    if (!ctx.automata_file_str) {
        return ERR_FILE_NOT_FOUND;
    }

    automaton_nfa = init_union_nfa(ctx.automata_file_str);
    if (!is_valid_nfa(automaton_nfa, ctx.categories_file_str)) {
        return ERR_AUTOMATA_NOT_LOADED;
    }
    
    scanner = scan_string(automaton_nfa, ctx.input_file_str);
    
    ctx.output_file_str = to_scanner_string(scanner);
    ctx.output_file = concat_strings(ctx.input_file, SCANNER_OUTPUT_SUFFIX);
    write_file(ctx.output_file, ctx.output_file_str);
    
    printf("Input file: %s\n", ctx.input_file);
    printf("Output file: %s\n", ctx.output_file);
    printf("Lexical analysis done!\n");

    clear_context(&ctx);
    delete_nfa(&automaton_nfa);
    delete_scanner(&scanner);
    return 0;
}