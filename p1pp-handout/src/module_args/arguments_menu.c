/**
 * @title: Argumennts Menu
 * @brief: Implenentation of arguments parsing.
 * @authors: Marc Bosch Manzano
 * @creation: 10/01/2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments_menu.h"
#include "../module_comments/comments.h"
#include "../module_directives/directives.h"
#include "../utils/file_utils.h"
#include "../utils/string_utils.h"
#include "../utils/errors_utils.h"

int is_empty(Flags flags) {
    return !(flags.comment || flags.directive || flags.help);
}

Flags process_arguments(int argc, char *argv[]) {
    Flags flags = { 0 };
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            flags.comment = 1;
        }
        else if (strcmp(argv[i], "-d") == 0) {
            flags.directive = 1;
        }
        else if (strcmp(argv[i], "-help") == 0) {
            flags.help = 1;
        }
        else if (strcmp(argv[i], "-all") == 0) {
            flags.comment = 1;
            flags.directive = 1;        
        }
        else {
            flags.file_position = i;
        }
    }

    if (is_empty(flags)) {
        flags.comment = 1;
    }
    return flags;
}

void execute_arguments(int argc, char *argv[]) {
    const Flags flags = process_arguments(argc, argv);

    if (flags.help) {
        char path[128];
        sprintf(path, MANUAL_PAGE, getenv("HOME"));
        print_file(path);
        return;
    }

    if (!flags.file_position) {
        printf(FILE_NOT_PROVIDED);
        return;
    }

    char *input_path = get_copy(argv[flags.file_position]);
    char *temp_path = add_suffix_to_path(input_path, "_temp");
    char *output_path = add_suffix_to_path(input_path, "_pp");

    char *current_input = NULL;
    char *current_output = NULL;

    if (flags.comment) {
        current_input = input_path;
        current_output = flags.directive ? temp_path : output_path;

#if (WARNING == ON)
        PRINT("Call -c");
        PRINT(current_input);
        PRINT(current_output);
#endif
        process_comments(current_input, current_output);
    }

    if (flags.directive) {
        current_input = current_output;
        current_output = output_path;

#if (WARNING == ON)
        PRINT("Call -d");
        PRINT(current_input);
        PRINT(current_output);
#endif
        process_directives(current_input, current_output);
    }    

    if (flags.comment && flags.directive) {
        remove(temp_path);
    }
    
    free(input_path);
    free(temp_path);
    free(output_path);

#if (WARNING == ON)
    PRINT("Program finished!");
#endif
}