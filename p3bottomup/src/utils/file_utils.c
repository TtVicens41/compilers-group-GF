/**
 * @title: File Utilities for P3 Bottom-Up Parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_utils.h"

FILE *open_file_or_null(const char *path, const char *mode) {
    if (!path || !mode) {
        return NULL;
    }
    return fopen(path, mode);
}

char *build_output_path_with_suffix(
    const char *input_path,
    const char *suffix,
    const char *extension
) {
    const char *last_slash;
    const char *last_dot;
    size_t stem_len;
    size_t suffix_len;
    size_t ext_len;
    char *output;

    if (!input_path || !suffix || !extension) {
        return NULL;
    }

    last_slash = strrchr(input_path, '/');
    last_dot = strrchr(input_path, '.');

    if (!last_dot || (last_slash && last_dot < last_slash)) {
        stem_len = strlen(input_path);
    } else {
        stem_len = (size_t)(last_dot - input_path);
    }

    suffix_len = strlen(suffix);
    ext_len = strlen(extension);

    output = (char *)calloc(stem_len + suffix_len + ext_len + 1, sizeof(char));
    if (!output) {
        return NULL;
    }

    memcpy(output, input_path, stem_len);
    memcpy(output + stem_len, suffix, suffix_len);
    memcpy(output + stem_len + suffix_len, extension, ext_len);
    output[stem_len + suffix_len + ext_len] = '\0';

    return output;
}
