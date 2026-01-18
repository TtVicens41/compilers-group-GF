#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "context.h"

#define MAX_LINE 1024

/*
 * run_preprocessor
 * ----------------
 * Executes the preprocessing engine over the input file
 * using the configuration stored in the context.
 */
void run_preprocessor(PreprocessorContext *ctx);

#endif
