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
#include "../language_defs.h"

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

char *trim(const char *str) {
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
    // Trim leading
    char *start = str;
    while (isspace(*start)) start++;
    
    // Trim trailing
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    
    // Move and null-terminate
    size_t len = end - start + 1;
    memmove(str, start, len);
    str[len] = '\0';
}