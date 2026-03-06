/**
 * @file string_utils.h
 * @brief String Utilities
 * 
 * Declarations of functions and macro definitions of string handling 
 * utilities. Most string functions get as argument an string and return 
 * another string. Take account dinamyc allocated strings have to be freed.
 * 
 * @authors Marc Bosch Manzano & Pau Puig Guillén
 * @since 2026-02-25
 * @see Improved from P2 Lexer string_utils.h
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "../language_defs.h"

/** To easily print a string into stdout with newline. */
#define PRINT_STR(s)({printf("%s\n", (char *)(s));})

/** To easily print an integer into stdout with newline. */
#define PRINT_INT(i)({printf("%d\n", (int)(i));})

/** To easily print a character into stdout with newline. */
#define PRINT_CHAR(c)({printf("%c\n", (char)(c));})

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
 * Concatenate two strings
 * @param[in] left_string A pointer to read-only string.
 * @param[in] right_string A pointer to read-only string.
 * @return A pointer to a dinamically allocated string of concatenated 
 * strings.
 */
char *concat_strings(const char *left_string, const char *right_string);

/**
 * Retrieves the position where a trim character is found.
 * @param[in] string A pointer to read-only character array.
 * @return The position fom 0 to N - 1 where trim is found; returns 0 if it 
 * is not found the trim.
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

/**
 * Gets the raw representation of spcecial characters. Converts the characters
 * '\n', '\t', '\r', \ into strings of the literal string representation of them.
 * For the rest of characters, it returns a string of the character as is.
 * @param character An ASCII character.
 * @return A dynamic allocated string of the raw representation of the character.
 */
char *get_raw_char(char character);

/**
 * Says whether the string is empty or not.
 * @param[in] string A read-only character array.
 * @return More than 1 when string is not empty, 0 otherwise.
 */
int is_not_empty(const char *string);

#endif
