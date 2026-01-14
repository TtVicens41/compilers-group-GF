/**
 * @brief
 * @author: Marc
 * @date 
 */

#include <stdio.h>
#include "comments.h"
#include "string_utils.h"
#include "errors_handling.h"

void delete_comments(
    const char *input_path, 
    const char *output_path
) {
    FILE *input_file = fopen(input_path, "r");
    if (!input_file) {
        printf(OPEN_FAILURE, input_path);
        return;
    }

    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        printf(OPEN_FAILURE, output_path);
        return;
    }

    char c;
    char next;
    while ((c = fgetc(input_file)) != EOF) {
        if (c != '/') {
            fputc(c, output_file);
            continue;
        }

        next = fgetc(input_file);
        if (next == '/') {
            while ((c = fgetc(input_file)) != EOF && (c != '\n'));
            fputc('\n', output_file);
            continue;
        }

        if (next == '*') {
            while ((c = fgetc(input_file)) != EOF) {
                if (c == '*') {
                    if ((next = fgetc(input_file)) == '/') {
                        break;
                    }
                }
            }
            continue;
        }

        fputc(c, output_file);
        if (next != EOF) {
            fputc(next, output_file);
        }
    }

    fclose(input_file);
    fclose(output_file);    
}


/// TODO: review this function.
/// @brief Function that writes only non-empty lines (lines containing 
/// a non-whitespace character).
void delete_empty_rows(
    const char *input_path, 
    const char *output_path
) 
{
    FILE *input_file = fopen(input_path, "r");
    if (!input_file) {
        printf(OPEN_FAILURE, input_path);
        return;
    }

    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        printf(OPEN_FAILURE, output_path);
        fclose(input_file);
        return;
    }

    char line[4096];
    while (fgets(line, sizeof(line), input_file)) {
        int i = 0;
        int has_non_ws = 0;
        while (line[i] != '\0') {
            if (!isspace((unsigned char)line[i])) {
                has_non_ws = 1;
                break;
            }
            i++;
        }
        if (has_non_ws) {
            fputs(line, output_file);
        }
    }

    fclose(input_file);
    fclose(output_file);
}

void process_comments(
    const char *input_path, 
    const char *output_path
) {
    char *temp_path = add_suffix_to_path(input_path, "_temp");

    delete_comments(input_path, temp_path);
    delete_empty_rows(temp_path, output_path);
}