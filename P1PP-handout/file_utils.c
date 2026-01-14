#include <stdio.h>

#include "file_utils.h"

void print_file(const char *file_name) {
    FILE *file_ptr = fopen(file_name, "r");
    if (!file_ptr) {
        return;
    }

    char c;
    while ((c = fgetc(file_ptr)) != EOF) {
        fputc(c, stdout);
    }

    fclose(file_ptr);
}

void copy_file(
    const char *input_file_name,
    const char *output_file_name
) {
    FILE *input_file_ptr = fopen(input_file_name, "r");
    if (!input_file_ptr) {
        return;
    }

    FILE *output_file_ptr = fopen(output_file_name, "r");
    if (!output_file_ptr) {
        fclose(input_file_ptr);
        return;
    }

    char c;
    while ((c = fgetc(input_file_ptr)) != EOF) {
        fputc(c, output_file_ptr);
    }

    fclose(input_file_ptr);
    fclose(output_file_ptr);
}