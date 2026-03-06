/**
 * @file file_utils.h
 * @brief File I/O Utilities Module
 * 
 * All functions recieve a path, or both input and output paths, as parameters. 
 * The main logic is to read from that input path, and wirte content to that 
 * output path.
 * 
 * @author Marc Bosch Manzano
 * @since 2026-02-16
 * @see Improved from P1 Preprocessor file_utils.h
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../language_defs.h"

/**
 * Prints a file content into standard output.
 * @param path File path to be printed.
 */
void print_file(const char *path);

/**
 * Reads the full content of a file into a string.
 * @param[in] path Path from we read content.
 * @return A dynamically string containing the full file body.
 */
char *read_file(const char *path);

/**
 * Writes string into a file, overwritting it.
 * @param[in] path Path from we write content.
 * @param[in] string A read-only character array.
 */
void write_file(const char *path, const char *string);

/**
 * Reads the full content of a file into a string.
 * @param[in] path Path from we read content.
 * @param[inout] str An overwritten string to dump the body of the file.
 * @param[in] max_len Maximum characters to be read.
 */
void dump_file(const char *path, char *str, int max_len);

/**
 * Copies file content to another file, overwritting it.
 * @param input_path Path from we read content.
 * @param output_path Path from we write content.
 */
void copy_file(const char *input_path, const char *output_path);

/**
 * Function used to check wether a file can be open in read mode.
 * @param input_path File to be read.
 * @return 1 if file correctly opened, 0 otherwise.
 * @note Prints the error reason in case the file cannot be open.
 */
int check_input_file(const char *input_path);

/**
 * Function used to check wether a file can be open in write mode.
 * @param output_path File to be write.
 * @return 1 if file correctly opened, 0 otherwise.
 * @note Prints the error reason in case the file cannot be open.
 */
int check_output_file(const char *output_path); 

#endif // FILE_UTILS_H
