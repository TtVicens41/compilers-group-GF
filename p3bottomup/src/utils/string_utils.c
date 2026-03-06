/**
 * @file string_utils.c
 * @brief String Utilities.
 * @authors Marc Bosch Manzano, Pau Puig Guillén, Davi Penna-Mattos Dias de 
 * Paiva.
 * @since 2026-02-25.
 * @see Improved from P2 Lexer string_utils.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "string_utils.h"

#define IS_WHITESPACE(c) (\
    (c) == ' ' ||\
    (c) == '\t' ||\
    (c) == '\n' ||\
    (c) == '\r'\
)
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

char *get_copy(const char *string) {
    if (!string) return NULL;
    char *buffer = calloc(strlen(string) + 1, sizeof(char));
    strcpy(buffer, string);
    return buffer;
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

char *concat_strings(const char *left_string, const char *right_string) {
    const size_t left_length = strlen(left_string);
    const size_t right_length = strlen(right_string);
    const size_t concat_length = left_length + right_length + 1;

    char *concat_string = calloc(concat_length, sizeof(char));
    if (!concat_string) { return NULL; }
    strcat(concat_string, left_string);
    strcat(concat_string, right_string);

    return concat_string;
}

void append_suffix(char **string, const char *suffix) {
    const size_t string_length = *string ? strlen(*string) : 0;
    const size_t suffix_length = strlen(suffix);
    const size_t concat_length = string_length + suffix_length + 1;

    char *tmp = realloc(*string, concat_length * sizeof(char));
    if (!tmp) { 
        return; 
    }

    if (string_length == 0) {
        tmp[0] = '\0';
    }

    strcat(tmp, suffix);
    *string = tmp;
}

void append_prefix(const char *prefix, char **string) {
    const size_t prefix_length = strlen(prefix);
    const size_t string_length = *string ? strlen(*string) : 0;
    const size_t concat_length = prefix_length + string_length + 1;

    char *tmp1 = calloc(concat_length, sizeof(char));
    if (!tmp1) { return; }
    strcat(tmp1, prefix);
    strcat(tmp1, *string);

    char *tmp2 = realloc(*string, concat_length * sizeof(char));
    if (!tmp2) { return; }
    strcpy(tmp2, tmp1);

    *string = tmp2;
    free(tmp1);
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

char *replace_extension(const char *path, const char *suffix)
{
    char *prefix = get_prefix_r(path, EXTENSION_SEPARATOR);
    char *new_path = concat_strings(prefix, suffix);
    free(prefix);
    return new_path;
}

int find_trim(const char *string) {
    if (!string) { return 0; }

    size_t length = strlen(string);
    for (size_t i = 0; i < length; i++) {
        if (IS_WHITESPACE(string[i]))
            return i;
    }
    return 0;
}

char *copy_until_trim(const char *string) {
    if (!string) { return NULL; }

    size_t len = 0;
    while (string[len] && !IS_WHITESPACE(string[len])) { 
        len++; 
    }

    char *new_string = malloc(len + 1);
    if (!new_string) { return NULL; }

    memcpy(new_string, string, len);
    new_string[len] = '\0';

    return new_string;
}

char *trim_string(const char *string) {
    if (!string) { return NULL; }

    while (IS_WHITESPACE(*string)) 
        string++;

    if (*string == '\0') {
        return get_copy(string);
    }
    
    char *new_string = get_copy(string);
    char *end = new_string + strlen(new_string) - 1;
    while (end > new_string && IS_WHITESPACE(*end)) 
        end--;
    *(end + 1) = '\0';

    return new_string;
}

void trim_whitespace(char *string) {
    char *start = string;
    while (isspace(*start)) 
        start++;
    
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) 
        end--;
    
    size_t len = end - start + 1;
    memmove(string, start, len);
    string[len] = '\0';
}

char *get_raw_char(char c) {
    char *out = calloc(3, sizeof(char));
    size_t j = 0;
    switch (c) {
        case '\0': out[j++] = '\\'; out[j++] = '0'; break;
        case '\a': out[j++] = '\\'; out[j++] = 'a'; break;
        case '\b': out[j++] = '\\'; out[j++] = 'b'; break;
        case '\t': out[j++] = '\\'; out[j++] = 't'; break;
        case '\n': out[j++] = '\\'; out[j++] = 'n'; break;
        case '\v': out[j++] = '\\'; out[j++] = 'v'; break;
        case '\f': out[j++] = '\\'; out[j++] = 'f'; break;
        case '\r': out[j++] = '\\'; out[j++] = 'r'; break;
        default: out[j++] = c;
    }
    return out;
}

char *get_raw_string(const char *string) {
    size_t length = strlen(string);
    char *out = calloc(length * 2 + 1, sizeof(char));
    if (!out) {
        return NULL;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < length; i++) {
        char c = string[i];
        switch (c) {
            case '\0': out[j++] = '\\'; out[j++] = '0'; break;
            case '\a': out[j++] = '\\'; out[j++] = 'a'; break;
            case '\b': out[j++] = '\\'; out[j++] = 'b'; break;
            case '\t': out[j++] = '\\'; out[j++] = 't'; break;
            case '\n': out[j++] = '\\'; out[j++] = 'n'; break;
            case '\v': out[j++] = '\\'; out[j++] = 'v'; break;
            case '\f': out[j++] = '\\'; out[j++] = 'f'; break;
            case '\r': out[j++] = '\\'; out[j++] = 'r'; break;
            default: out[j++] = c;
        }
    }

    out[j] = '\0';
    return out;
}

int is_not_empty(const char *string) {
    return (int)strlen(string); 
}

char *ptr_to_str(const void *pointer) {
    if (!pointer) {
        return NULL;
    }
    int len = snprintf(NULL, 0, "\"%p\"", pointer);
    char *string = calloc(len + 1, sizeof(char));
    if (!string) {
        return NULL;
    }
    snprintf(string, len + 1, "\"%p\"", pointer);
    return string;
}

char *add_quotes(const char *string) {
    if (!string) { 
        return NULL; 
    }
    size_t len = strlen(string);
    char *new_string = calloc(len + 3, sizeof(char));
    sprintf(new_string, "\"%s\"", string);
    return new_string;
}

void jsonify(
    char **string,
    int indentation_level, 
    int is_comma,
    int is_newline,
    const char *attribute, 
    char *value 
) {
    char *indent = get_spaces(indentation_level * INDENTATION);
    append_suffix(string, indent);
    append_suffix(string, "\"");
    append_suffix(string, attribute);
    append_suffix(string, "\": ");
    append_suffix(string, value ? value : "null");
    append_suffix(string, is_comma ? ", " : "");
    append_suffix(string, is_newline ? "\n" : "");
    free(indent);
    free(value);
}

void jsonify_value(
    char **string,
    int indentation_level, 
    int is_comma,
    int is_newline,
    char *value
) {
    char *indent = get_spaces(indentation_level * INDENTATION);
    append_suffix(string, indent);
    append_suffix(string, value ? value : "null");
    append_suffix(string, is_comma ? ", " : "");
    append_suffix(string, is_newline ? "\n" : "");
    free(indent);
    free(value);
}

void jsonify_wrap(
    char **string,
    int level,
    int is_newline,
    const char *wrapper 
) {
    char *indent = get_spaces(level * INDENTATION);
    append_suffix(string, indent);
    append_suffix(string, wrapper);
    append_suffix(string, is_newline ? "\n" : "");
    free(indent);
}

char *add_indentation(const char *string, int level) {
    char *indent = get_spaces(level * INDENTATION);
    char *new_string = concat_strings(indent, string);
    free(indent);
    return new_string;
}

char *get_spaces(size_t num_spaces) {
    char *string = calloc(num_spaces + 1, sizeof(char));
    size_t i = 0;
    while (i < num_spaces) {
        string[i++] = ' ';
    }
    string[i] = '\0';
    return string;
}

char *int_to_str(int integer) {
    int len = snprintf(NULL, 0, "%d", integer);
    char *string = calloc(len + 1, sizeof(char));
    if (!string) { 
        return NULL; 
    }
    sprintf(string, "%d", integer);
    return string;
}

char *int_array_to_str(const int *array, int count) {
    if (!array) {
        return NULL;
    }
    char *string = NULL;
    append_suffix(&string, "[");
    for (int i = 0; i < count; i++) {
        append_suffix(&string, int_to_str(array[i]));
        if (i < count - 1) {
            append_suffix(&string, ", ");
        }
    }
    append_suffix(&string, "]");
    return string;
}

char *int_matrix_to_str(int* const *matrix, int rows, int columns, int indentation_level) {
    if (!matrix) {
        return NULL;
    }
    char *string = get_copy("\n");
    jsonify_wrap(&string, indentation_level, 1, "[");
    for (int i = 0; i < rows; i++) {
        jsonify_value(
            &string, 
            indentation_level + 1, 
            i < rows - 1,
            1, 
            int_array_to_str(matrix[i], columns)
        );
    }
    jsonify_wrap(&string, indentation_level, 0, "]");
    return string;
}

char *trim(char *s)
{
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') s++;
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        *end-- = '\0';
    return s;
}

int is_blank(const char *s)
{
    while (*s) {
        if (*s != ' ' && *s != '\t' && *s != '\r' && *s != '\n') 
            return 0;
        s++;
    }
    return 1;
}

int is_blank_or_comment(const char *line)
{
    while (*line == ' ' || *line == '\t') line++;
    return (*line == '\0' || *line == '\n' || *line == '#');
}