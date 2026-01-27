/**
 * @title: Argumennts Menu.
 * @brief: Declarations of the arguments parsing utilities.
 * @authors: Alejandro Poole Becerra.
 * @creation: before 2026/01/27.
 */

#ifndef PARSE_ARGUMENTS_H
#define PARSE_ARGUMENTS_H

#include <stdio.h>

#include "context.h"

#define FLAG_HELP "-help"
#define FLAG_COMMENTS "-c"
#define FLAG_DIRECTIVES "-d"
#define FLAG_ALL "-all"

/**
 * Parses the argument characters initializing the preprocessor context.
 * @param argc Argument count, i.e, number of strings in `argv`.
 * @param argv Argument vector, i.e., a string array.
 * @param ctx A pointer to a preprocessor context.
 */
void parse_arguments(int argc, char *argv[], PreprocessorContext *ctx);

#endif