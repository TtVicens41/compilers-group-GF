/**
 * @file parse_arguments.h
 * @brief Argumennts Menu
 * @authors Marc Bosch Manzano, Joan Vicente Martín & Pau Puig Guillén
 * @since 2026-01-15
 * @see: Adapted from P1 Preprocessor parse_arguments.h
 */


#ifndef PARSE_ARGUMENTS_H
#define PARSE_ARGUMENTS_H

/**
 * @brief Parses the argument characters.
 * @param argc Argument count, i.e, number of strings in `argv`.
 * @param argv Argument vector, i.e., a string array.
 * @return Input file for the lexer, if providen.
 */
char *parse_arguments(int argc, char *argv[]);

void print_usage(const char *argv0);
char *resolve_resource_path(const char *argv0, const char *resource_name);

#endif
