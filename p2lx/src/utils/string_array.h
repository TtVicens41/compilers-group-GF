/**
 * @file string_array.h
 * @brief String Array Utilities
 * 
 * A string array implemented as a dynamic continguous array of strings (aka
 * character arrays terminated with `'\0'`), and using a size value to control 
 * the number of strings. 
 * 
 * This module provides functions that accept strings and generate string list,
 * like "string split". It also handles functions that take string arrays and
 * generate new string arrays by filtering or aplying a transformation to them.
 * 
 * @author Marc Bosch Manzano
 * @since 2026-02-09
 */

#ifndef STRING_LIST_H
#define STRING_LIST_H

/**
 * @brief String array data structure.
 */
typedef struct {
    char **buffer;  /* A string array. */
    int size;       /* Number of strings in string array. */
} StringArray;

/**
 * @brief Initializes the attributes of the string list.
 * @param buffer A string array.
 * @param size The number of strings in string array.
 * @return A dynamic allocated string list.
 */
StringArray *init_string_array(char **buffer, int size);

/**
 * @brief Splits a string into an array of strings given a separator character.
 * @param string A pointer to a read-only array of characters.
 * @param splitter A character that splits the string.
 * @return A dynamic allocated string with splitted substrings.
 */
StringArray *string_split(const char *string, char splitter); 

/**
 * @brief Frees the fields of the string list, without freeing istelf.
 * @param array A pointer to string array with dynamically allocated 
 * fields.
 */
void clear_string_array(StringArray *array);

/**
 * @brief Frees the fields and the string list itself. 
 * 
 * The dereferenced pointer of string list is set to NULL at the end, 
 * preventing it is used again.
 * 
 * @param array A pointer to a pointer of a dynamically allocated 
 * string list. 
 */
void delete_string_array(StringArray **array);

/**
 * @brief Creates a new string list by applying a tranformation to each of 
 * its strings.
 * @param array A pointer to a string list.
 * @param applier A pointer to a function that maps string to string. 
 * @param delete A flag that deletes the input string list when it is not 0.
 * @return A dynamic allocated string array with its transformed strings.
 */
StringArray *apply_string_array(
    StringArray *array, 
    char *(*applier)(const char *), 
    int delete
);

/**
 * @brief Creates a new string list by filtering the strings by some condition. 
 * 
 * The resulting string list has at most the length of the input string list.
 * Also, input string can be deleted with delete flag enabled.
 *
 * @param array A pointer to a string list.
 * @param condition A pointer to a function returns evaluates some condition
 * to a string, returning a boolean.
 * @param delete A flag that deletes the input string list when it is not 0.
 * @return A dynamic allocated filtered string array.
 */
StringArray *filter_string_list(
    StringArray *array,
    int (*condition)(const char *),
    int delete
);

/**
 * @brief Prints strings line by line in a formatted way.
 * @param array A pointer to a read-only string list.
 * @param is_raw A boolean value, 0 or 1. The strings can be printed converting
 * its special characters to its is_raw format, e.g. `'\r'` or `'\t'` (`is_raw `
 * = 1). Otherwise, the special characters are rendered (`is_raw` = 0).
 */
void print_string_array(const StringArray *array, int is_raw);

/**
 * Creates an array of integers from a string list. Assumes the strings in the
 * list can be converted into an integer.
 * @param string A pointer to a string list. 
 * @param delete A flag that deletes the string list when it is not 0.
 * @return A dynamicallly allocated array of integers.
 */
int *to_integer_array(StringArray *array, int delete);

#endif // STRING_LIST_H
