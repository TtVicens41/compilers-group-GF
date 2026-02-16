/**
 * @title: parse_arguments.h
 * @authors: Joan Vicente, Pau Puig
 * @creation: 16/02/2025
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
