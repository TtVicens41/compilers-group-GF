/**
 * @title: String List Utilities.
 * @brief: Implementation of string list handling functions.
 * @authors: Marc Bosch Manzano
 * @creation: 2026/02/09
 */

/**
 * A string list implementation using a pointer to character
 * pointers and a size value to contrlor the number of strings.
 */
typedef struct {
    char **buffer; /// A sring array.
    int size; /// Number of strings in string array.
} StringList;

/**
 * Initializes the attributes of the string list.
 * @param[in] buffer A string array.
 * @param size The number of strings in string array.
 * @return A dynamic allocated string list.
 */
StringList *init_string_list(char **buffer, int size);

StringList *string_split(char *string, char splitter); 

StringList *apply_string_list(StringList *string_list, char *(*applier)(const char *), int delete);

void clear_string_list(StringList *string_list);

void delete_string_list(StringList **string_list);

void print_string_list(StringList *string_list, int raw);

int *to_integer_array(StringList *string_list, int delete);
