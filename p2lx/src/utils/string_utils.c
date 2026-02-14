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

/**
 * @brief Creates a heap copy of the input string.
 * @param Receives: `const char *string`.
 * @return Returns a newly allocated copy of `string`.
 * @details This helper centralizes allocation strategy for string clones.
 */
char *get_copy(const char *string) {
    char *buffer = calloc(strlen(string) + 1, sizeof(char));
    strcpy(buffer, string);
    return buffer;
}

/**
 * @brief Returns the suffix starting at the first occurrence of `separator`.
 * @param Receives: `const char *string, char separator`.
 * @return Returns an allocated suffix string.
 * @details If separator is missing, it returns a full copy of the input.
 */
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

/**
 * @brief Returns the suffix starting at the last occurrence of `separator`.
 * @param Receives: `const char *string, char separator`.
 * @return Returns an allocated suffix string.
 * @details If separator is missing, it returns a full copy of the input.
 */
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

/**
 * @brief Returns the prefix up to the first occurrence of `separator`.
 * @param Receives: `const char *string, char separator`.
 * @return Returns an allocated prefix string.
 * @details If separator is missing, it returns a full copy of the input.
 */
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

/**
 * @brief Returns the prefix up to the last occurrence of `separator`.
 * @param Receives: `const char *string, char separator`.
 * @return Returns an allocated prefix string.
 * @details If separator is missing, it returns a full copy of the input.
 */
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

/**
 * @brief Inserts `suffix` between file stem and extension in a path.
 * @param Receives: `const char *path, const char *suffix`.
 * @return Returns a newly allocated path string.
 * @details Used to generate derivative file names (`.cscn`, `_pp`, etc.).
 */
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

/**
 * @brief Finds the first whitespace position in a string.
 * @param Receives: `const char *string`.
 * @return Returns the index of first trim char, or `0` if not found.
 * @details This helper is kept for compatibility with existing modules.
 */
int find_trim(const char *string) {
    size_t length;
    size_t i;

    if (!string) {
        return 0;
    }

    length = strlen(string);
    for (i = 0; i < length; i++) {
        if (IS_WHITESPACE(string[i]))
            return (int)i;
    }
    return 0;
}

/**
 * @brief Copies characters until the first whitespace character.
 * @param Receives: `const char *string`.
 * @return Returns an allocated prefix without trailing trim chars.
 * @details Useful for quick token extraction in utility contexts.
 */
char *copy_until_trim(const char *string) {
    char *copy = calloc(BUFFER_SIZE_LINE, sizeof(char));
    int i = 0;
    while (*string && !IS_WHITESPACE(*string)) {
        copy[i++] = *string++;
    }
    copy[i] = '\0';
    return copy;
}

/**
 * @brief Returns a new string with leading/trailing whitespace removed.
 * @param Receives: `const char *str`.
 * @return Returns an allocated trimmed copy.
 * @details Keeps original source string immutable.
 */
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

/**
 * @brief Trims leading and trailing whitespace in place.
 * @param Receives: `char *str`.
 * @return Does not return a value.
 * @details Mutates the provided buffer and preserves internal content order.
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

/**
 * @brief Extracts substring from `start` to `end` (both inclusive).
 * @param Receives: `const char *string, int start, int end`.
 * @return Returns an allocated substring.
 * @details Input bounds are clamped to valid string limits.
 */
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

/**
 * @brief Escapes control characters for debug-friendly rendering.
 * @param Receives: `const char *string`.
 * @return Returns a newly allocated escaped string.
 * @details Converts `\n`, `\t`, `\r`, and `\\` into visible sequences.
 */
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
