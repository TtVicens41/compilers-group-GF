#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdbool.h>
#include "directives/define/define.h"

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

    bool output_enabled;

    /* Symbol table for #define macros */
    DefineTable define_table;

    bool in_block_comment;

} PreprocessorContext;

#endif