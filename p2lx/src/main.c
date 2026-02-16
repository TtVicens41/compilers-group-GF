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

#include "./parse_arguments/parse_arguments.h"
#include "./language_defs.h"
#include "./utils/utils.h"
#include "./automata/automata.h"
#include "./scanner/scanner.h"

static int COUNT = 0;

int main(int argc, char *argv[]) {
    char *input_file_name;
    char *input_file_str;
    char *automata_input_file;
    char *automata_file_str;
    char *scanner_str;
    char *output_file_name;
    Scanner *scanner;
    NFA *automaton_nfa;
    
    input_file_name = parse_arguments(argc, argv);
    input_file_str = read_file(input_file_name);
    if (!input_file_str) {
        return ERR_FILE_NOT_FOUND;
    }
    
    automata_input_file = concat_strings(get_resources_dir(), AUTOMATA_FILE);
    automata_file_str = read_file(automata_input_file);
    if (!automata_file_str) {
        return ERR_FILE_NOT_FOUND;
    }

    automaton_nfa = init_union_nfa(automata_file_str);
    if (!is_valid_nfa(automaton_nfa)) {
        return ERR_AUTOMATA_NOT_LOADED;
    }
    
    scanner = scan_string(automaton_nfa, input_file_str);
    scanner_str = get_formatted_scanner(scanner);

    output_file_name = concat_strings(input_file_name, SCANNER_OUTPUT_SUFFIX);
    write_file(output_file_name, scanner_str);
    
    printf("Input file: %s\n", input_file_name);
    printf("Output file: %s\n", output_file_name);
    printf("Lexical analysis done!\n");

    free(input_file_name);
    free(input_file_str);
    free(automata_input_file);
    free(automata_file_str);
    free(scanner_str);
    free(output_file_name);
    delete_nfa(&automaton_nfa);
    delete_scanner(&scanner);
    return 0;
}