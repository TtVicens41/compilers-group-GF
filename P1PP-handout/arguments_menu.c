#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comments.h"
#include "directives.h"
#include "string_utils.h"
#include "file_utils.h"
#include "arguments_menu.h"
#include "errors_handling.h"

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
    return flags;
}

void execute_arguments(int argc, char *argv[]) {
    Flags flags = process_arguments(argc, argv);

    if (flags.help) {
        print_file(MANUAL_PAGE);
        return;
    }

    if (!flags.file_position) {
        printf(FILE_NOT_PROVIDED);
        return;
    }

    char *input_path = get_copy(argv[flags.file_position]);
    char *output_path = add_suffix_to_path(input_path, "_pp");

    if (flags.comment) {
        printf("Call -c\n");
        process_comments(input_path, output_path);
    }

    if (flags.directive) {
        printf("Call -d\n");
        process_directives(input_path, output_path);
    }
}