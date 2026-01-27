/**
 * Comments Processor Module
 * @author: Marc Bosch Manzano & Pol Goicoechea Esparza
 * @creation: 09/01/2026
 * @revisions: (14/01/2026) by authors. Added documentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "comments.h"
#include "../utils/string_utils.h"
#include "../utils/errors_utils.h"
#include "../utils/file_utils.h"

void delete_comments(
    const char *input_path, 
    const char *output_path
) 
{
    if(!check_input_file(input_path) || !check_output_file(output_path))
        return;
        
    FILE *input_file = fopen(input_path, "r");
    FILE *output_file = fopen(output_path, "w");

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

void delete_empty_rows(
    const char *input_path, 
    const char *output_path
) 
{
    if(!check_input_file(input_path) || !check_output_file(output_path))
        return;
        
    FILE *input_file = fopen(input_path, "r");
    FILE *output_file = fopen(output_path, "w");

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
    char *temp_path = add_suffix_to_path(output_path, "_temp");

    delete_comments(input_path, temp_path);
    delete_empty_rows(temp_path, output_path);
    
    remove(temp_path);
    free(temp_path);
}