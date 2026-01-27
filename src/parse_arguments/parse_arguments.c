#include "parse_arguments.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void print_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void parse_arguments(int argc, char *argv[], PreprocessorContext *ctx){
    ctx->remove_comments = false;
    ctx->process_directives = false;
    ctx->help_request = false;
    
    bool has_flags = false;

    for(int i = 1; i<argc;i++){
        if(strcmp(argv[i],"-c") == 0){
            ctx->remove_comments = true;
            has_flags = true;
        }
        else if(strcmp(argv[i],"-d") == 0){
            ctx->process_directives = true;
            has_flags = true;
        }
        else if(strcmp(argv[i],"-help") == 0){
            ctx->help_request = true;
            print_file(MANUAL_PAGE);
            return;
        }
        else if(strcmp(argv[i],"-all") == 0){
            ctx->remove_comments = true;
            ctx->process_directives = true;
            has_flags = true;
        }
    }
    
    if(!has_flags){
        ctx->remove_comments = true;
    }
}
