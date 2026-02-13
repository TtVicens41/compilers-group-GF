/**
 * @title: string_utils.c
 * @authors:
 * @creation:
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
        return get_copy(str);
    
    char *str_copy = get_copy(str);
    char *end = str_copy + strlen(str_copy) - 1;
    while (end > str_copy && IS_WHITESPACE(*end)) 
        end--;

    *(end + 1) = '\0';
    return str_copy;
}

/**
 * @brief Explica la responsabilidad de `trim_whitespace` en el flujo del compilador.
 * @param Recibe: `char *str`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
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
    int length = (int)strlen(string);
    end = min(end, length - 1);
    start = max(start, 0);
    char *aux = (char *)calloc((end - start + 2), sizeof(char));
    for (int i = start; i <= end; i++) {
        aux[i - start] = string[i];
    }
    return aux;
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
