/**
 * @title: Argumennts Menu.
 * @brief: Implementations of arguments parsing utilities.
 * @authors: Alejandro Poole Becerra.
 * @creation: before 2026/01/27.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse_arguments.h"
#include "../language_defs.h"

void parse_arguments(int argc, char *argv[], PreprocessorContext *ctx){
    ctx->remove_comments = false;
    ctx->process_directives = false;
    ctx->help_request = false;
    bool has_flags = false;

    for(int i = 1; i<argc;i++){
        if(strcmp(argv[i], FLAG_COMMENTS) == 0){
            ctx->remove_comments = true;
            has_flags = true;
        }
        else if (strcmp(argv[i], FLAG_DIRECTIVES) == 0){
            ctx->process_directives = true;
            has_flags = true;
        }
        else if (strcmp(argv[i], FLAG_HELP) == 0){
            ctx->help_request = true;
        }
        else if (strcmp(argv[i], FLAG_ALL) == 0){
            ctx->remove_comments = true;
            ctx->process_directives = true;
            has_flags = true;
        }
    }
    
    if(!has_flags){
        ctx->remove_comments = true;
    }

    ctx->current_line = 0;
    ctx->output_enabled = true;
    ctx->in_block_comment = false;
    ctx->ifdef_depth = 0;
    ctx->ifdef_skip_depth = 0;

    // Find input filename
    ctx->input_filename = NULL;
    for(int i = argc - 1; i >= 1; i--){
        if(argv[i][0] != FLAG_PREFIX){
            ctx->input_filename = argv[i];
            break;
        }
    }
}
