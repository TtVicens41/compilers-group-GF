
#include "parse_arguments.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

    for(int i = 1; i<argc;i++){

        if(strcmp(argv[i],"-c") == 0){
            ctx->remove_comments = true;
        }
        else if(strcmp(argv[i],"-d") == 0){
            ctx->process_directives = true;
        }
        else if(strcmp(argv[i],"-help") == 0){
            print_file(MANUAL_PAGE);
        }
        else if(strcmp(argv[i],"-all") == 0){
            ctx->remove_comments = true;
            ctx->process_directives = true;
        }
    }

}