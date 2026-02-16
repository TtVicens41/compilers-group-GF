/**
 * Argumennts Menu.
 * 
 * @description: Declarations of the arguments parsing utilities.
 * @authors: Marc Bosch Manzano
 * @creation: 2026/01/15.
 * @see_also: Adapted from P1 Preprocessor.
 */

#ifndef PARSE_ARGUMENTS_H
#define PARSE_ARGUMENTS_H

/**
 * Parses the argument characters initializing the preprocessor context.
 * @param argc Argument count, i.e, number of strings in `argv`.
 * @param argv Argument vector, i.e., a string array.
 * @param ctx A pointer to a preprocessor context.
 */
char *parse_arguments(int argc, char *argv[]);

#endif