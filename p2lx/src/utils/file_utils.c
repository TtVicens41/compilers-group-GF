/**
 * @title: file_utils.c
 * @authors:
 * @creation:
 */

#include <stdio.h>
#include <stdlib.h>

#include "file_utils.h"
#include "error_utils.h"

/**
 * @brief Describes the responsibility of `print_file` in the compiler pipeline.
 * @param Receives: `const char *path`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void print_file(const char *path) {
    FILE *file_ptr;
    int c;

    if (!check_input_file(path)) {
        return;
    }

    file_ptr = fopen(path, "r");
    if (!file_ptr) {
        return;
    }

    while ((c = fgetc(file_ptr)) != EOF) {
        fputc(c, stdout);
    }

    fclose(file_ptr);
}

char *read_file(const char *path) {
    FILE *file_ptr;
    long size;
    long bytes;
    char *buffer;

    if (!check_input_file(path)) {
        return NULL;
    }

    file_ptr = fopen(path, "r");
    if (!file_ptr) {
        return NULL;
    }

    if (fseek(file_ptr, 0, SEEK_END) != 0) {
        fclose(file_ptr);
        return NULL;
    }

    size = ftell(file_ptr);
    if (size < 0) {
        fclose(file_ptr);
        return NULL;
    }

    rewind(file_ptr);

    buffer = (char *)calloc((size_t)size + 1, sizeof(char));
    if (!buffer) {
        fclose(file_ptr);
        return NULL;
    }

    bytes = (long)fread(buffer, sizeof(char), (size_t)size, file_ptr);
    buffer[bytes] = '\0';

    fclose(file_ptr);
    return buffer;
}

/**
 * @brief Describes the responsibility of `dump_file` in the compiler pipeline.
 * @param Receives: `const char *path, char *str, int max_len`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void dump_file(const char *path, char *str, int max_len) {
    FILE *file_ptr;
    int i = 0;
    int c;

    if (!check_input_file(path)) {
        return;
    }

    file_ptr = fopen(path, "r");
    if (!file_ptr) {
        return;
    }

    while ((c = fgetc(file_ptr)) != EOF && i < max_len - 1) {
        str[i++] = (char)c;
    }
    str[i] = '\0';

    fclose(file_ptr);
}

/**
 * @brief Describes the responsibility of `copy_file` in the compiler pipeline.
 * @param Receives: `const char *input_path, const char *output_path`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void copy_file(const char *input_path, const char *output_path) {
    FILE *input_file_ptr;
    FILE *output_file_ptr;
    int c;

    if (!check_input_file(input_path) || !check_output_file(output_path)) {
        return;
    }

    input_file_ptr = fopen(input_path, "r");
    output_file_ptr = fopen(output_path, "w");
    if (!input_file_ptr || !output_file_ptr) {
        if (input_file_ptr) {
            fclose(input_file_ptr);
        }
        if (output_file_ptr) {
            fclose(output_file_ptr);
        }
        return;
    }

    while ((c = fgetc(input_file_ptr)) != EOF) {
        fputc(c, output_file_ptr);
    }

    fclose(input_file_ptr);
    fclose(output_file_ptr);
}

/**
 * @brief Describes the responsibility of `check_input_file` in the compiler pipeline.
 * @param Receives: `const char *input_path`.
 * @return Returns a value of type `int`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
int check_input_file(const char *input_path) {
    FILE *input_file = fopen(input_path, "r");
    if (!input_file) {
        print_file_error(input_path);
        return 0;
    }
    fclose(input_file);
    return 1;
}

/**
 * @brief Describes the responsibility of `check_output_file` in the compiler pipeline.
 * @param Receives: `const char *output_path`.
 * @return Returns a value of type `int`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
int check_output_file(const char *output_path) {
    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        print_file_error(output_path);
        return 0;
    }
    fclose(output_file);
    return 1;
}
