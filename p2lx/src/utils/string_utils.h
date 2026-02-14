/**
 * @title: string_utils.h
 * @authors:
 * @creation:
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "../language_defs.h"

/** To easily print a string into stdout. */
#define PRINT(s) ({printf("%s\n", s);})

/**
 * Copies a string into a new one.
 * @param[in] string A pointer to read-only character array.
 * @return A pointer to a dinamically allocated character array being its size
 * the length of its characters.
 */
char *get_copy(const char *string);

/**
 * Gets the substring from the start of the string to the first occurrency of 
 * the separator. 
 * @param[in] string A pointer to read-only character array.
 * @param separator A character.
 * @return A pointer to a dinamically allocated string of the leftmost prefix.
 */
char *get_prefix(const char *string, char separator);

/**
 * Gets the substring from the start of the string to the last occurrency of 
 * the separator. 
 * @param[in] string A pointer to read-only character array.
 * @param separator A character.
 * @return A pointer to a dinamically allocated string of the rightmost prefix.
 */
char *get_prefix_r(const char *string, char separator);

/**
 * Gets the substring from the first occurrency of the separator to the end of
 * the string.
 * @param[in] string A pointer to read-only character array.
 * @param separator A character.
 * @return A pointer to a dinamically allocated string of the leftmost suffix.
 */
char *get_suffix(const char *string, char separator);

/**
 * Gets the substring from the last occurrency of the separator to the end of
 * the string.
 * @param[in] string A pointer to read-only character array.
 * @param separator A character.
 * @return A pointer to a dinamically allocated string of the rightmost suffix.
 */
char *get_suffix_r(const char *string, char separator);

/**
 * Adds a suffix to a path. The suffix is appended between the file name 
 * -without the extension- and the extension.
 * @param[in] path A pointer to read-only string.
 * @param[in] suffix A suffix to append to the path.
 * @return A pointer to a dinamically allocated string of the new path.
 */
char *add_suffix_to_path(const char *path, const char *suffix);

/**
 * Retrieves the position where a trim character is found.
 * @param[in] string A pointer to read-only character array.
 * @return The position fom 0 to N - 1 where trim is found; returns 0 if it 
 * is not found the trim.
 */
/**
 * @brief Describes the responsibility of `find_trim` in the compiler pipeline.
 * @param Receives: `const char *string`.
 * @return Returns a value of type `int`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
int find_trim(const char *string);

/**
 * Gets the substring from the start to the first trim character detected.
 * @param[in] string A pointer to read-only character array.
 * @return A pointer to a dinamically allocated string of the substring.
 */
char *copy_until_trim(const char *string);

/**
 * Remove leading and trailing spaces, tabs, newlines, or carriage returns 
 * from a string.
 * @param[in] str A pointer to read-only character array.
 * @return A pointer to a dinamically allocated string trimmed substring.
 */
char *trim_string(const char *str);

/**
 * Trim leading and trailing whitespace in place.
 * @param[inout] str A pointer to read-write character array.
 */
/**
 * @brief Describes the responsibility of `trim_whitespace` in the compiler pipeline.
 * @param Receives: `char *str`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void trim_whitespace(char *str);

/**
 * Convert a string formatting the control characters '\n', '\t', etc. 
 * to its raw repsresentation.
 * @param[in] string A poiner to a read only string.
 * @return A pointer to a dinamically allocated string raw string.
 */
char *get_raw_string(const char *string);

/**
 * Get a substring in a string from start to end positions, including them.
 * @param[in] string A poiner to a read only string.
 * @return A pointer to a dinamically allocated substring.
 */
char *get_substring(const char *string, int start, int end);

#endif
