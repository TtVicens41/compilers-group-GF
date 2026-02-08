/**
 * @title: String Utilities.
 * @brief: Implementation of string handling functions.
 * @authors: Marc Bosch Manzano & Pau Puig Guillén
 * @creation: 10/01/2026
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "string_utils.h"

char *get_copy(const char *string) {
    char *buffer = calloc(strlen(string) + 1, sizeof(char));
    strcpy(buffer, string);
    return buffer;
}

char *get_suffix(const char *string, char separator) {
    if (!string) {
        return NULL;
    }
    char *suffix = strchr(string, separator);
    if (!suffix) {
        return get_copy(string);
    }
    return get_copy(suffix);
}

char *get_suffix_r(const char *string, char separator) {
    if (!string) {
        return NULL;
    }
    char *suffix = strrchr(string, separator);
    if (!suffix) {
        return get_copy(string);
    }
    return get_copy(suffix);
}

char *get_prefix(const char *string, char separator) {
    if (!string) {
        return NULL;
    }
    char *suffix = strchr(string, separator);
    if (!suffix) {
        return get_copy(string);
    }
    int length = suffix - string;
    char *buffer = calloc(length + 1, sizeof(char));
    memcpy(buffer, string, length);
    buffer[length] = '\0';
    return buffer;
}

char *get_prefix_r(const char *string, char separator) {
    if (!string) {
        return NULL;
    }
    char *suffix = strrchr(string, separator);
    if (!suffix) {
        return get_copy(string);
    }
    int length = suffix - string;
    char *buffer = calloc(length + 1, sizeof(char));
    memcpy(buffer, string, length);
    buffer[length] = '\0';
    return buffer;
}

char *add_suffix_to_path(const char *path, const char *suffix) {
    if (!path) {
        return NULL;
    }
    if (!suffix) {
        return get_copy(path);
    }

    char *directory = get_prefix_r(path, PATH_SEPARATOR);
    char *file_name = get_suffix_r(path, PATH_SEPARATOR);
    if (strcmp(file_name, directory) == 0) {
        directory = get_copy("\0");
    }

    char *file_stem = get_prefix_r(file_name, EXTENSION_SEPARATOR);
    char *extension = get_suffix_r(file_name, EXTENSION_SEPARATOR);
    if (strcmp(file_stem, extension) == 0) {
        extension = get_copy("\0");
    }

    char *new_path = calloc(strlen(path) + strlen(suffix) + 1, sizeof(char));
    strcat(new_path, directory);
    strcat(new_path, file_stem);
    strcat(new_path, suffix);
    strcat(new_path, extension);

    return new_path;
}

int find_trim(const char *string) {
    if (!string) {
        NULL;
    }
    size_t length = strlen(string);
    for (int i = 0; i < length; i++) {
        if (IS_WHITESPACE(string[i]))
            return i;
    }
    return 0;
}

char *copy_until_trim(const char *string) {
    char *copy = calloc(BUFFER_SIZE_LINE, sizeof(char));
    int i = 0;
    while (*string && !IS_WHITESPACE(*string)) {
        copy[i++] = *string++;
    }
    copy[i] = '\0';
    return copy;
}

char *trim_string(const char *str) {
    if (!str) {
        return NULL;
    }

    while (IS_WHITESPACE(*str)) 
        str++;

    if (*str == '\0') 
        return get_copy(str);
    
    char *str_copy = get_copy(str);
    char *end = str_copy + strlen(str_copy) - 1;
    while (end > str_copy && IS_WHITESPACE(*end)) 
        end--;

    *(end + 1) = '\0';
    return str_copy;
}

void trim_whitespace(char *str) {
    char *start = str;
    while (isspace(*start)) 
        start++;
    
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) 
        end--;
    
    size_t len = end - start + 1;
    memmove(str, start, len);
    str[len] = '\0';
}

char *get_substring(const char *string, int start, int end) {
    int length = strlen(string);
    end = min(end, length - 1);
    start = max(start, 0);
    char *aux = (char *)calloc((end - start + 2), sizeof(char));
    for (int i = start; i <= end; i++) {
        aux[i - start] = string[i];
    }
    return aux;
}

StringList *init_string_list(char **buffer, int size) {
    StringList *string_list = malloc(sizeof(StringList));
    string_list->buffer = buffer;
    string_list->size = size;
    return string_list;
}

// StringList *string_split(char *string, char splitter) {
//     int length = strlen(string);
//     char **buffer = (char **)calloc(1, sizeof(char *));
//     int size = 0;
//     int start = 0;
//     for (int i = 1; i < length - 1; ++i) {
//         if ((string[i] != splitter) && (string[i - 1] == splitter)) {
//             start = i;
//         }
//         if ((string[i + 1] == splitter) && (string[i] != splitter)) {
//             buffer = (char **)realloc(buffer, (size + 1) * sizeof(char *));
//             buffer[size] = get_substring(string, start, i);
//             size++;
//         }
//     }
//     if (string[length - 1] != splitter) {
//         buffer = (char **)realloc(buffer, (size + 1) * sizeof(char *));
//         buffer[size] = get_substring(string, start, length - 1);
//         size++;
//     }
//     return init_string_list(buffer, size);
// }

#define UNSET_TOKEN (-1)

int is_token_set(int token_start) {
    return token_start != UNSET_TOKEN;
}

int reset_token(int *token_start) {
    *token_start = UNSET_TOKEN;
}

int is_split_condition(char c, char splitter) {
    return c == splitter || c == '\0';
}

StringList *string_split(char *string, char splitter) {
    int length = strlen(string);
    
    char **buffer = NULL;
    int size = 0;   
    int token_start = UNSET_TOKEN;

    for (int i = 0; i <= length; i++) {
        if (!is_split_condition(string[i], splitter)) {
            if (!is_token_set(token_start)) {
                token_start = i; 
            }
        } else {
            if (is_token_set(token_start)) {
                buffer = realloc(buffer, (size + 1) * sizeof(char *));
                buffer[size++] = get_substring(string, token_start, i - 1);
                reset_token(&token_start);
            }
        }
    }

    return init_string_list(buffer, size);
}

StringList *apply_string_list(
    StringList *string_list, 
    char *(*applier)(const char *),
    int delete
) {
    char **buffer = calloc(string_list->size, sizeof(char *));
    int size = string_list->size;
    for (int i = 0; i < size; i++) {
        buffer[i] = applier(string_list->buffer[i]);
    }
    if (delete) {
        delete_string_list(&string_list);
    }
    return init_string_list(buffer, size);
}

void clear_string_list(StringList *string_list) {
    for (int i = 0; i < string_list->size; i++) {
        free(string_list->buffer[i]);
        string_list->buffer[i] = NULL;
    }
    free(string_list->buffer);
    string_list->buffer = NULL;
    string_list->size = 0;
}

void delete_string_list(StringList **string_list) {
    clear_string_list(*string_list);
    free(*string_list);
    *string_list = NULL;
}

void print_string_list(StringList *string_list, int raw) {
    if (!string_list) { return; }
    if (!string_list->buffer) { return; }

    printf("[\n");
    for (int i = 0; i < string_list->size; i++) {
        if (raw) {
            printf("\"%s\"", get_raw_string(string_list->buffer[i]));
        } else {
            printf("%s", string_list->buffer[i]);
        }

        if (i < string_list->size - 1) {
            printf(",\n");
        }
    }
    printf("\n]\n");
}

char *get_raw_string(const char *string) {
    int length = strlen(string);
    char *out = calloc(length * 2 + 1, sizeof(char));
    if (!out) {
        return NULL;
    }

   size_t j = 0;
    for (size_t i = 0; i < length; i++) {
        char c = string[i];
        switch (c) {
            case '\n': out[j++] = '\\'; out[j++] = 'n'; break;
            case '\t': out[j++] = '\\'; out[j++] = 't'; break;
            case '\r': out[j++] = '\\'; out[j++] = 'r'; break;
            case '\\': out[j++] = '\\'; out[j++] = '\\'; break;
            default: out[j++] = c;
        }
    }

    out[j] = '\0';
    return out;
}

int *to_integer_array(StringList *string_list, int delete) {
    int size = string_list->size;
    int *array = calloc(string_list->size, sizeof(int));
    for (int i = 0; i < string_list->size; i++) {
        array[i] = atoi(string_list->buffer[i]);
    }
    if (delete) {
        delete_string_list(&string_list);
    }
    return array;
}