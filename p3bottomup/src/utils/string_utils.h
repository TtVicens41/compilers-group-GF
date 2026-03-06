/**
 * @file string_utils.h
 * @brief String Utilities.
 * 
 * Declarations of functions and macro definitions of string handling 
 * utilities. Most string functions get as argument an string and return 
 * another string. Take account dinamyc allocated strings have to be freed.
 
 * @authors Marc Bosch Manzano, Pau Puig Guillén, Davi Penna-Mattos Dias de 
 * Paiva.
 * @since 2026-02-25.
 * @see Improved from P2 Lexer string_utils.h
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

/** 
 * Level of indentation for string functions and procedures that handle string 
 * indentation.
  */
#define INDENTATION 4

/** Commonly used path separator character. */
#define PATH_SEPARATOR          '/'

/** Commonly used extension separator character. */
#define EXTENSION_SEPARATOR     '.'

/** To easily print a string into stdout with newline. */
#define PRINT_STR(s)({printf("%s\n", (char *)(s));})

/** To easily print an integer into stdout with newline. */
#define PRINT_INT(i)({printf("%d\n", (int)(i));})

/** To easily print a character into stdout with newline. */
#define PRINT_CHAR(c)({printf("%c\n", (char)(c));})

/**
 * Copies a string into a new one.
 * @param[in] string A pointer to read-only character array.
 * @return A a dinamically allocated character array being its size the length
 * of its characters.
 */
char *get_copy(const char *string);

/**
 * Gets the substring from the start of the string to the first occurrency of 
 * the separator. 
 * @param[in] string A pointer to read-only character array.
 * @param separator A character.
 * @return A dinamically allocated string of the leftmost prefix.
 */
char *get_prefix(const char *string, char separator);

/**
 * Gets the substring from the start of the string to the last occurrency of 
 * the separator. 
 * @param[in] string A pointer to read-only character array.
 * @param separator A character.
 * @return A dinamically allocated string of the rightmost prefix.
 */
char *get_prefix_r(const char *string, char separator);

/**
 * Gets the substring from the first occurrency of the separator to the end of
 * the string.
 * @param[in] string A pointer to read-only character array.
 * @param separator A character.
 * @return A dinamically allocated string of the leftmost suffix.
 */
char *get_suffix(const char *string, char separator);

/**
 * Gets the substring from the last occurrency of the separator to the end of
 * the string.
 * @param[in] string A pointer to read-only character array.
 * @param separator A character.
 * @return A dinamically allocated string of the rightmost suffix.
 */
char *get_suffix_r(const char *string, char separator);

/**
 * Get a substring in a string from start to end positions, including them.
 * @param[in] string A poiner to a read-only string.
 * @return A dinamically allocated substring.
 */
char *get_substring(const char *string, int start, int end);

/**
 * Concatenate two strings into a new allocated string.
 * @param[in] left_string A pointer to read-only character array.
 * @param[in] right_string A pointer to read-only character array.
 * @return A dinamically allocated string (NULL when no available heap memory).
 */
char *concat_strings(const char *left_string, const char *right_string);

/**
 * Append a suffix into a string.
 * @param[inout] string A pointer to read-write character array.
 * @param[in] suffix A read-only character array.
 * @note When no available heap memory to do the append, the input string is 
 * not modified.
 */
void append_suffix(char **string, const char *suffix);

/**
 * Append a prefix into a string.
 * @param[in] prefix A read-only character array.
 * @param[inout] string A pointer to read-write character array.
 * @note When no available heap memory to do the append, the input string is 
 * not modified.
 */
void append_prefix(const char *prefix, char **string);

/**
 * Adds a suffix to a path, where the suffix is appended between the file 
 * name -without the extension- and the extension.
 * @param[in] path A pointer to read-only character array representing a path.
 * @param[in] suffix A suffix to append to the path.
 * @return A dinamically allocated string of the new path.
 */
char *add_suffix_to_path(const char *path, const char *suffix);

/**
 * Replace the extension (or append a new one) by some suffix.
 * @param[in] path An input file path with our without extension.
 * @param[in] suffix A string for suffix to be appended to the input_path when
 * removing the extension.
 * @return A dynamically allocated string of the new path version.
 */
char *replace_extension(const char *path, const char *suffix);

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
 * @param[in] string A pointer to read-only character array.
 * @return A pointer to a dinamically allocated string trimmed substring.
 */
char *trim_string(const char *string);

/**
 * Trim leading and trailing whitespace in place.
 * @param[inout] string A pointer to read-write character array.
 */
void trim_whitespace(char *string);

/**
 * @brief Gets the raw representation of spcecial characters. 
 * 
 * Converts the characters '\n', '\t', '\r' into strings of the literal string 
 * representation of them. For the rest of characters, it returns a string of 
 * the character as is.
 * 
 * @param character An ASCII character.
 * @return A dynamic allocated string of the raw representation of the character.
 */
char *get_raw_char(char character);

/**
 * Convert a string formatting the control characters '\n', '\t', etc. 
 * to its raw repsresentation.
 * @param[in] string A poiner to a read only string.
 * @return A pointer to a dinamically allocated string raw string.
 */
char *get_raw_string(const char *string);

/**
 * Says whether the string is empty or not.
 * @param[in] string A read-only character array.
 * @return More than 1 when string is not empty, 0 otherwise.
 */
int is_not_empty(const char *string);

/**
 * String representation of an integer.
 * 
 */
char *int_to_str(int integer);

/**
 * String representation of a pointer.
 * @param pointer A generic read-only pointer
 * @return A dinamically allocated string representing the pointer (NULL when 
 * no-pointer).
 */
char *ptr_to_str(const void *pointer);

/**
 * Add quotes at the beggining and the end of a string: `\"...\"`.
 * @param string A read-only string.
 * @return A new string with the quotes added. 
 */
char *add_quotes(const char *string);

/**
 * @brief Appends to a string the JSON attribute-value representation. 
 * 
 * The format is given as [\"`attribute`\": `value`]. The `indentation_level` 
 * specifies the number of spaces appendet at the beggining of the JSON line. 
 * It adds `level` times `INDENTATION` number of spaces, where the indentation 
 * is defined to be a constant (usually 2, 4, or 8). The caller can append a 
 * comma or an newline character at the end of the attribute line enabling 
 * the flags that cotntrol this behaviour.
 * 
 * @param[inout] string A pointer to a read-write string which it is appended 
 * as a suffix the attribute-value representation.
 * @param indentation_level An integer representing the indentation level of
 * the JSON object (0, 1, ...).
 * @param is_comma A boolean that appends a comma whit space [`, `] when true.
 * @param is_newline A boolean that apends an new line [`\n`] character when 
 * ture.
 * @param[in] attribute A read-only string representing an attribute of JSON 
 * object.
 * @param[inout] value A dinamically allocated string representing the value 
 * of the attribute, automatically freed inside the function.
 */
void jsonify(
    char **string,
    int indentation_level, 
    int is_comma,
    int is_newline,
    const char *attribute, 
    char *value 
) ;

/**
 * @brief Appends to a string the JSON list value representation.
 * 
 * For JSON lists `[ ]`, values are concatenated one whith another, and no 
 * attribute is specified for a value. A value can be either a string, a number
 * a null, another object, another list, or a boolean. This function only 
 * accpets the string representation of the value, which is assumed that is
 * assummed to be jsonified.
 * 
 * @param[inout] string A pointer to a read-write string which it is appended 
 * as a suffix the attribute-value representation.
 * @param indentation_level An integer representing the indentation level of
 * the JSON object (0, 1, ...).
 * @param is_comma A boolean that appends a comma whit space [`, `] when true.
 * @param is_newline A boolean that apends an new line [`\n`] character when 
 * ture.
 * @param[inout] value A dinamically allocated string representing the value 
 * of the attribute, automatically freed inside the function.
 */
void jsonify_value(
    char **string,
    int indentation_level, 
    int is_comma,
    int is_newline,
    char *value
);

/**
 * @brief Appends a wrap character into a string.
 * 
 * In fact, this function can append any string to the main string. However, 
 * the caller often use this function to append  a JSON wrapper character
 * to the main string: `{`, `}`, `[`, or `]`.
 * 
 * @param[inout] string A pointer to a read-only string which it is appended 
 * as a suffix the attribute-value representation.
 * @param indentation_level An integer representing the indentation level of
 * the JSON object (0, 1, ...).
 * @param is_newline A boolean that apends an new line [`\n`] character when 
 * ture.
 * @param[in] wrapper A read-only string, usually representing a wrapper.
 */
void jsonify_wrap(
    char **string,
    int indentation_level,
    int is_newline,
    const char *wrapper 
);

/**
 * @brief Add spaces at the beggining of the string.
 * 
 * The number of spaces are given by the `indentation_level` times
 * a constant value `INDENTATION` (usually 2, 4, or 8).
 * 
 * @param[in] string A read-only string.
 * @param indentation_level The indentation level of the string (0, 1, ...).
 * @return A dinaimcally allocated indented string with spaces.
 */
char *add_indentation(const char *string, int indentation_level);

/**
 * Get a string of space characters only.
 * @param num_spaces The number of spaces of the string.
 * @return A dinamically allocated string of spaces.
 */
char *get_spaces(size_t num_spaces);

/**
 * A jsonified string representation of an integer array.
 * @param array A read-only integer one-dimensional array.
 * @param count Number of elements of the integer array.
 */
char *int_array_to_str(const int *array, int count);

/**
 * A jsonified string representation of an integer matrix.
 * @param matrix A read-only integer two-dimensional array.
 * @param rows Number of rows of the matrix.
 * @param columns Number of columns of the matrix.
 * @param indentation_level An integer representing the indentation level of
 * the JSON object (0, 1, ...).
 */
char *int_matrix_to_str(
    int* const *matrix, 
    int rows, 
    int columns, 
    int indentation_level
);

/** Trim leading/trailing whitespace in place and return the pointer. */
char *trim(char *s);

/** True when string has a blank character, false otherwise. */
int is_blank(const char *s);

/** True when string has a blank character or a #-comment, false otherwise. */
int is_blank_or_comment(const char *line);

#endif
