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
void delete_empty_rows(
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
    char prev;
    while ((c = fgetc(input_file)) != EOF) {
        prev = c;
        while(((c = fgetc(input_file)) != EOF) && (c == '\n'));
        fputc(prev, output_file);
        fputc(c, output_file);
        // if (c == '\n') {
        //     next = fgetc(input_file);
        //     if (next == '\n') {
        //         continue;
        //     } 
        //     else {
        //         fputc(c, output_file);
        //         fputc(next, output_file);
        //     }
            
        // } else {
        //     fputc(c, output_file);
        // }
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