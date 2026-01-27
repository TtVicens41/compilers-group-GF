/**
 * @title: Keyword Dispatcher Directives.
 * @brief: Declarations of arguments parsing utilities.
 * @author: Alejandro Poole Becerra & Marc Bosch Manzano.
 * @creation: before 2026/01/27.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdbool.h>

#include "symbol_table/symbol_table.h"

#define MANUAL_PAGE "%s/.local/share/preprocessor/man_page.txt"

/**
 * PreprocessorContext
 * -------------------
 * Holds all shared state required by the preprocessing engine. This structure
 * is passed to all modules to avoid global state and to allow extensibility 
 * of the engine.
 */
typedef struct {
    /// An input file pointer. 
    FILE *input;    

    /// An output file pointer. 
    FILE *output;   

    /// An input file path string. 
    const char *input_filename; 
    
    /// An output file path string. 
    const char *output_filename; 

    /// The current reading line in a file.
    int current_line; 

    /// Flag to remove comments.
    bool remove_comments; 

    /// Flag to process directives.
    bool process_directives; 

    /// Flag to print manual page.
    bool help_request; 

    /// Flag to enable the output file.
    bool output_enabled; 

    /// Flag to know wether we are in block comment of C (with asterisks).
    bool in_block_comment; 

    /// A pointer to a table of symbols of the `#define` directive.
    SymbolTable *symbol_table; 

    /// Current nesting level of the directive `#ifdef`.
    int ifdef_depth; 

    /// Depth at which we started skipping on `#ifdef` directive.
    int ifdef_skip_depth; 

} PreprocessorContext;


#endif