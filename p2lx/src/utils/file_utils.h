/**
 * @title: file_utils.h
 * @authors:
 * @creation:
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../language_defs.h"

/**
 * @brief Describes the responsibility of `print_file` in the compiler pipeline.
 * @param Receives: `const char *path`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void print_file(const char *path);
char *read_file(const char *path);
/**
 * @brief Describes the responsibility of `dump_file` in the compiler pipeline.
 * @param Receives: `const char *path, char *str, int max_len`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void dump_file(const char *path, char *str, int max_len);
/**
 * @brief Describes the responsibility of `copy_file` in the compiler pipeline.
 * @param Receives: `const char *input_path, const char *output_path`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void copy_file(const char *input_path, const char *output_path);
/**
 * @brief Describes the responsibility of `check_input_file` in the compiler pipeline.
 * @param Receives: `const char *input_path`.
 * @return Returns a value of type `int`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
int check_input_file(const char *input_path);
/**
 * @brief Describes the responsibility of `check_output_file` in the compiler pipeline.
 * @param Receives: `const char *output_path`.
 * @return Returns a value of type `int`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
int check_output_file(const char *output_path);

#endif
