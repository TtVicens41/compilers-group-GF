/**
 * @file arguments.h
 * @brief Module for parse of command-line arguments 
 * 
 * This module reads argument count and argument vector. It checks user has 
 * introduced the correct arguments: the language, the parse table files, 
 * the .cscn file to be parsed, and the file for debugging. 
 * 
 * @author Marc Bosch Manzano
 * @since 2026/02/25
 */

#ifndef ARGUMENTS_H
#define ARGUMENTS_H


#include "../language/language.h"
#include "../language/grammar.h"
#include "../automaton/parse_table.h"
#include "../parser/sra.h"
#include "../automaton/dfa.h"
#include "../stack/stack.h"

static const char PATH_MAN_PAGE[] = "./resources/man_page.txt";
static const char PATH_LANGUAGE[] = "./resources/grammar_b/language.txt";
static const char PATH_PARSE_TABLE[] = "./resources/grammar_b/parse_table.txt";
static const char DEBUG_SUFFIX[] = "_p3dbg.txt";

/**
 * @brief Parser context data structure.
 * 
 * Stores all the necessary fields to be allocated for the parser and handles 
 * its deallocation. Furthermore, controls the execution of the program, and
 * handles when the parser has to execute a cancellation point.
 */
typedef struct {
    const char *man_page;      /* Path to the manual page. */
    const char *language_file; /* Path to the language definition file.*/
    const char *table_file;    /* Path to the parse-table file. */
    const char *input_file;    /* Path to the .cscn token file. */
    const char *output_file;   /* Path for the debug-output file. */     
    Language   *lang;          /* A language with symbols and productions. */
    ParseTable *ptable;        /* A parse table with actions and gotos. */
    DFA        *dfa;           /* A deterministic finite-state automata. */
    Stack      *stack;         /* A stack data structure. */
    SRA        *sra;           /* A shift-reduce automata. */
    TokenList  *tokens;        /* A list of tokens parsed from input file. */
    Logger     *logger;        /* A logger to handle output file. */
    int         status;        /* The status of the program execution (0 or 1). */
} ParserContext;

/**
 * Parses the parser command-line arguments.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param ctx Parser context assumed to be allocated in memory and set to null
 * all its fields. 
 * @return EXIT_SUCESS (0) or EXIT_FAILURE (1).
 */
void parse_arguments(int argc, char *argv[], ParserContext *ctx);

/**
 * A cancellation point of the program when contxet status is EXIT_FAILURE.
 * @param ctx A parser context assumed to be allocated in memory. 
 */
void context_exit(const ParserContext *ctx);

/**
 * Frees the dynamic allocated fields of the parser context without deleting
 * itself.
 * @param ctx A parser context assumed to be allocated in memory. 
 */
void context_cleanup(ParserContext *ctx);

/**
 * Get a string representation of the parser context.
 * @param ctx A read-only parser context. 
 * @return A dynamically allocated string representing the context.
 */
char *context_string(const ParserContext *ctx, int is_indent);

#endif /* ARGUMENTS_H */
