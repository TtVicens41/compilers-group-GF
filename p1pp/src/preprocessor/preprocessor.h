/**
 * @title: Run Preprocessor.
 * @brief: Declares the main function to run the preprocessor
 * using the context data structure.
 * @author: Davi Panna-Mattos Dias de Paiva
 * @creation: before 2026/01/27
 */

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "context.h"
#include "../language_defs.h"

/**
 * Executes the preprocessing engine over the input file using the 
 * configuration stored in the context. 
 * 
 * The prerpocess that it calls are the following, depending on the context:
 * 
 * - comments: remove comments.
 * 
 * - directives: replace directives by its content.
 * 
 * @param ctx Pointer to context data structure, containing the 
 * input file.
 */
void run_preprocessor(PreprocessorContext *ctx);

#endif
