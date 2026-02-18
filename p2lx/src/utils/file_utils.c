/**
 * File I/O Handling Utilities.
 * 
 * @description: Implements file I/O handling functions.
 * @author: Marc Bosch Manzano & Pol Goicoechea Esparza
 * @creation: 09/01/2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./file_utils.h"
#include "./error_utils.h"

void print_file(
    const char *path
) {
    if (!check_input_file(path)) 
        return;

    FILE *file_ptr = fopen(path, "r");
    char c;
    while ((c = fgetc(file_ptr)) != EOF) {
        fputc(c, stdout);
    }
    fclose(file_ptr);
}

void dump_file(const char *path, char *str, int max_len) {
    if (!check_input_file(path)) 
        return;

    FILE *file_ptr = fopen(path, "r"); 
    int i = 0;
    char c = 0;

    while ((c = fgetc(file_ptr)) != EOF && i < max_len - 1) {
        str[i++] = c;
    }
    str[i] = '\0';
    fclose(file_ptr);
}

char *read_file(const char *path) {
    if (!check_input_file(path))
        return NULL;
    
    FILE *file_ptr = fopen(path, "r");
    const int m = BUFFER_SIZE_MEDIUM;
    char *str = NULL;
    char c = 0;
    int i = 0;

    while ((c = fgetc(file_ptr)) != EOF) {
        if ((i % m) == 0) {
            char *tmp = (char *)realloc(str, (i + 1) * m);
            if (!tmp) {
                break;
            }
            str = tmp;
        }
        str[i++] = c;
    }

    fclose(file_ptr);
    str[i] = '\0';
    return str;
}

void write_file(const char *path, const char *string) {
    if (!check_output_file(path) || !string)
        return;
    
    FILE *file_ptr = fopen(path, "w");
    const size_t str_length = strlen(string);
    
    for (size_t i = 0; i < str_length; i++) {
        fputc(string[i], file_ptr);
    }
}


void copy_file(
    const char *input_path,
    const char *output_path
) {
    if (!check_input_file(input_path) || !check_output_file(output_path))
        return;
        
    FILE *input_file_ptr = fopen(input_path, "r");
    FILE *output_file_ptr = fopen(output_path, "w");

    char c;
    while ((c = fgetc(input_file_ptr)) != EOF) {
        fputc(c, output_file_ptr);
    }

    fclose(input_file_ptr);
    fclose(output_file_ptr);
}

int check_input_file(
    const char *input_path
) {
    FILE *input_file = fopen(input_path, "r");
    if (!input_file) {
        print_file_error(input_path);
        return 0;
    }
    fclose(input_file);
    return 1;
}

int check_output_file(
    const char *output_path
) {
    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        print_file_error(output_path);
        return 0;
    }
    fclose(output_file);
    return 1;
}