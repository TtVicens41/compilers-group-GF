/**
 * @title: String Utilities.
 * @brief: Declarations of functions and macro definitions of string handling 
 *      utilities. Most string functions get as argument an string and return
 *      another string. Take account dinamyc allocated strings have to be 
 *      freed.
 * @authors: Marc Bosch Manzano & Pau Puig Guillén
 * @creation: 10/01/2026
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

char *get_raw_string(const char *string);

typedef struct {
    char **buffer;
    int size;
} StringList;

char *get_substring(const char *string, int start, int end);
StringList *init_string_list(char **buffer, int size);
StringList *string_split(char *string, char splitter);
StringList *apply_string_list(StringList *string_list, char *(*applier)(const char *), int delete);
void clear_string_list(StringList *string_list);
void delete_string_list(StringList **string_list);
void print_string_list(StringList *string_list, int raw);
int *to_integer_array(StringList *string_list, int delete);

#endif