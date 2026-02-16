/**
 * @title: string_list.h
 * @authors: Alejandro Poole
 * @creation: 16/02/2026
 */


#ifndef STRING_LIST_H
#define STRING_LIST_H

/**
 * A string list data structure using a pointer to character pointers and a 
 * size value to control the number of strings.
 */
typedef struct {
    char **buffer;      /// A string array.
    int size;           /// Number of strings in string array.
} StringList;

/**
 * Initializes the attributes of the string list.
 * @param[in] buffer A string array.
 * @param size The number of strings in string array.
 * @return A dynamic allocated string list.
 */
StringList *init_string_list(char **buffer, int size);

/**
 * Splits a string into an array of strings given a separator character.
 * @param[in] string A pointer to a read-only array of characters.
 * @param splitter A character that splits the string.
 * @return A dynamic allocated string with splitted substrings.
 */
StringList *string_split(const char *string, char splitter); 

/**
 * Creates a new string list by applying a tranformation to each of its 
 * strings.
 * @param[inout] string_list A pointer to a string list.
 * @param applier A pointer to a function that maps string to string. 
 * @param delete A flag that deletes the input string list when it is not 0.
 * @return A dynamic allocated string with its transformed strings.
 */
StringList *apply_string_list(
    StringList *string_list, 
    char *(*applier)(const char *), 
    int delete
);

/**
 * Creates a new string list by filtering the strings by some condition. The 
 * resulting string list has at most the length of the input string list.
 * @param[inout] string_list A pointer to a string list.
 * @param condition A pointer to a function returns evaluates some condition
 * to a string, returning a boolean.
 * @param delete A flag that deletes the input string list when it is not 0.
 * @return A dynamic allocated string with its filtered strings.
 */
StringList *filter_string_list(
    StringList *string_list,
    int (*condition)(const char *),
    int delete
);

/**
 * Frees the fields of the string list, without freeing istelf.
 * @param[inout] string_list A pointer to string list with dynamically 
 * allocated fields.
 */
void clear_string_list(StringList *string_list);

/**
 * Frees the fields and the string list itself. The dereferenced pointer of 
 * string list is set to NULL at the end, preventing it is used again.
 * @param[inout] string_list A pointer to a pointer of a dynamically allocated 
 * string list. 
 */
void delete_string_list(StringList **string_list);

/**
 * Prints a string list in a formatted way. Each string is printed in a new 
 * line. 
 * @param[in] string_list A pointer to a read-only string list.
 * @param raw A boolean value, 0 or 1. The strings can be printed converting
 * its special characters to its raw format, e.g. `'\r'` or `'\t'` (`raw `= 1). 
 * Otherwise, the special characters are rendered (`raw` = 0).
 */
void print_string_list(const StringList *string_list, int raw);

/**
 * Creates an array of integers from a string list. Assumes the strings in the
 * list can be converted into an integer.
 * @param[inout] string A pointer to a string list. 
 * @param delete A flag that deletes the string list when it is not 0.
 * @return A dynamicallly allocated array of integers.
 */
int *to_integer_array(StringList *string_list, int delete);

#endif // STRING_LIST_H
