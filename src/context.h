#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdbool.h>

#define MANUAL_PAGE "%s/.local/share/preprocessor/man_page.txt"

// Forward declaration
typedef struct SymbolTable SymbolTable;

/*
 * PreprocessorContext
 * -------------------
 * Holds all shared state required by the preprocessing engine.
 * This structure is passed to all modules to avoid global state
 * and to allow extensibility of the engine.
 */
typedef struct {
    FILE *input;
    FILE *output;

    const char *input_filename;
    const char *output_filename;

    int current_line;

    bool remove_comments;
    bool process_directives;
    bool help_request;
    bool output_enabled;

    SymbolTable *symbol_table;

    bool in_block_comment;

    // ifdef nesting support
    int ifdef_depth;           // Current nesting level
    int ifdef_skip_depth;      // Depth at which we started skipping

} PreprocessorContext;

#endif