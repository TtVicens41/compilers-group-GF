/**
 * @title: string_list.c
 * @authors:
 * @creation:
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string_list.h"
#include "string_utils.h"

/**
 * @brief Creates a `StringList` wrapper around an existing string buffer.
 * @param Receives: `char **buffer, int size`.
 * @return Returns a newly allocated `StringList*`, or `NULL` on allocation failure.
 * @details This constructor centralizes list initialization and keeps ownership explicit.
 */
StringList *init_string_list(char **buffer, int size) {
    StringList *string_list = malloc(sizeof(StringList));
    if (!string_list) {
        return NULL;
    }
    string_list->buffer = buffer;
    string_list->size = size;
    return string_list;
}

#define UNSET_TOKEN (-1)

/**
 * @brief Checks whether a token start index is already initialized.
 * @param Receives: `int token_start`.
 * @return Returns `1` when token start is valid and `0` otherwise.
 * @details Used by `string_split` to detect token boundaries.
 */
static int is_token_set(int token_start) {
    return token_start != UNSET_TOKEN;
}

/**
 * @brief Resets the token start marker to the unset value.
 * @param Receives: `int *token_start`.
 * @return Does not return a value.
 * @details Keeps split-state updates consistent in one helper.
 */
static void reset_token(int *token_start) {
    *token_start = UNSET_TOKEN;
}

/**
 * @brief Evaluates whether the current character closes a token.
 * @param Receives: `char c, char splitter`.
 * @return Returns `1` when `c` is a split condition, `0` otherwise.
 * @details End-of-string is treated as a split condition to flush the last token.
 */
static int is_split_condition(char c, char splitter) {
    return c == splitter || c == '\0';
}

/**
 * @brief Splits a string by one delimiter character.
 * @param Receives: `char *string, char splitter`.
 * @return Returns a new `StringList*` containing token copies.
 * @details Empty fragments are ignored; only non-empty tokens are returned.
 */
StringList *string_split(char *string, char splitter) {
    int length;
    char **buffer = NULL;
    int size = 0;
    int token_start = UNSET_TOKEN;
    int i;

    if (!string) {
        return init_string_list(NULL, 0);
    }

    length = (int)strlen(string);

    for (i = 0; i <= length; i++) {
        if (!is_split_condition(string[i], splitter)) {
            if (!is_token_set(token_start)) {
                token_start = i;
            }
        } else if (is_token_set(token_start)) {
            char **grown = realloc(buffer, (size + 1) * sizeof(char *));
            if (!grown) {
                free(buffer);
                return NULL;
            }
            buffer = grown;
            buffer[size++] = get_substring(string, token_start, i - 1);
            reset_token(&token_start);
        }
    }

    return init_string_list(buffer, size);
}

/**
 * @brief Applies a transformation function to each element of a `StringList`.
 * @param Receives: `StringList *string_list, char *(*applier)(const char *), int delete`.
 * @return Returns a new transformed `StringList*`.
 * @details Optionally frees the input list when `delete` is true.
 */
StringList *apply_string_list(
    StringList *string_list,
    char *(*applier)(const char *),
    int delete
) {
    char **buffer;
    int size;
    int i;

    if (!string_list) {
        return NULL;
    }

    buffer = calloc(string_list->size, sizeof(char *));
    if (!buffer) {
        return NULL;
    }

    size = string_list->size;
    for (i = 0; i < size; i++) {
        buffer[i] = applier(string_list->buffer[i]);
    }

    if (delete) {
        delete_string_list(&string_list);
    }

    return init_string_list(buffer, size);
}

/**
 * @brief Frees all strings in a `StringList` but not the outer pointer itself.
 * @param Receives: `StringList *string_list`.
 * @return Does not return a value.
 * @details This is useful when ownership of the wrapper is handled elsewhere.
 */
void clear_string_list(StringList *string_list) {
    int i;
    if (!string_list) {
        return;
    }

    for (i = 0; i < string_list->size; i++) {
        free(string_list->buffer[i]);
        string_list->buffer[i] = NULL;
    }

    free(string_list->buffer);
    string_list->buffer = NULL;
    string_list->size = 0;
}

/**
 * @brief Fully destroys a `StringList` and nullifies the caller pointer.
 * @param Receives: `StringList **string_list`.
 * @return Does not return a value.
 * @details Combines buffer cleanup and wrapper deallocation safely.
 */
void delete_string_list(StringList **string_list) {
    if (!string_list || !*string_list) {
        return;
    }
    clear_string_list(*string_list);
    free(*string_list);
    *string_list = NULL;
}

/**
 * @brief Prints the content of a `StringList` to stdout.
 * @param Receives: `StringList *string_list, int raw`.
 * @return Does not return a value.
 * @details When `raw` is enabled, control characters are escaped for readability.
 */
void print_string_list(StringList *string_list, int raw) {
    int i;
    if (!string_list || !string_list->buffer) {
        return;
    }

    printf("[\n");
    for (i = 0; i < string_list->size; i++) {
        if (raw) {
            char *rendered = get_raw_string(string_list->buffer[i]);
            printf("\"%s\"", rendered ? rendered : "");
            free(rendered);
        } else {
            printf("%s", string_list->buffer[i]);
        }

        if (i < string_list->size - 1) {
            printf(",\n");
        }
    }
    printf("\n]\n");
}

/**
 * @brief Converts a `StringList` of numbers into an integer array.
 * @param Receives: `StringList *string_list, int delete`.
 * @return Returns a newly allocated `int*` with converted values.
 * @details Optionally frees the source `StringList` after conversion.
 */
int *to_integer_array(StringList *string_list, int delete) {
    int *array;
    int i;

    if (!string_list) {
        return NULL;
    }

    array = calloc(string_list->size, sizeof(int));
    if (!array) {
        return NULL;
    }

    for (i = 0; i < string_list->size; i++) {
        array[i] = atoi(string_list->buffer[i]);
    }

    if (delete) {
        delete_string_list(&string_list);
    }

    return array;
}
