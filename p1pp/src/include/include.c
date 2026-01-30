/**
 * @title: Include Directive Processing.
 * @authors: Joan Vicente Martín.
 * @creation: before 2026/01/27.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <libgen.h>
#include <stdlib.h>

#include "include.h"
#include "../preprocessor/preprocessor.h"
#include "../language_defs.h"

typedef struct {
    char files[MAX_INCLUDED_FILES][MAX_PATH_LENGTH];
    int count;
} IncludedFilesTracker;

static IncludedFilesTracker global_included_files = {{""}, 0};

static bool is_file_already_processed(const char *filepath) {
    for (int i = 0; i < global_included_files.count; i++) {
        if (strcmp(global_included_files.files[i], filepath) == 0) {
            return true;
        }
    }
    return false;
}

static void mark_file_as_processed(const char *filepath) {
    if (global_included_files.count < MAX_INCLUDED_FILES) {
        strncpy(global_included_files.files[global_included_files.count], 
                filepath, MAX_PATH_LENGTH - 1);
        global_included_files.files[global_included_files.count][MAX_PATH_LENGTH - 1] = '\0';
        global_included_files.count++;
    }
}

static bool extract_quoted_filename(const char *line, char *filename) {
    const char *start = strchr(line, CHAR_DOUBLE_QUOTE);
    if (!start) {
        return false;
    }
    
    start++;
    const char *end = strchr(start, CHAR_DOUBLE_QUOTE);
    if (!end || end == start) {
        return false;
    }
    
    int length = end - start;
    if (length >= MAX_FILENAME_IN_DIRECTIVE) {
        return false;
    }
    
    strncpy(filename, start, length);
    filename[length] = '\0';
    
    return true;
}

static bool is_angle_bracket_include(const char *line) {
    const char *p = line;
    
    while (*p && *p != CHAR_ANGLE_BRACKET_OPEN && *p != CHAR_DOUBLE_QUOTE) {
        p++;
    }
    
    return (*p == CHAR_ANGLE_BRACKET_OPEN);
}

static void resolve_include_path(const char *current_file, 
                                  const char *included_filename,
                                  char *resolved_path) {
    char *current_file_copy = strdup(current_file);
    char *dir = dirname(current_file_copy);
    
    snprintf(resolved_path, MAX_PATH_LENGTH, "%s/%s", dir, included_filename);
    
    free(current_file_copy);
}

static void insert_file_content_with_includes(PreprocessorContext *ctx, 
                                              const char *include_filepath) {
    FILE *included_file = fopen(include_filepath, "r");
    if (!included_file) {
        fprintf(stderr, "Error: Cannot open included file '%s'\n", include_filepath);
        return;
    }
    
    PreprocessorContext included_ctx = *ctx;
    included_ctx.input = included_file;
    included_ctx.input_filename = include_filepath;
    included_ctx.current_line = 0;
    
    run_preprocessor(&included_ctx);
    
    fclose(included_file);
}

void handle_include(PreprocessorContext *ctx, const char *line) {
    if (is_angle_bracket_include(line)) {
        if (ctx->output_enabled) {
            fputs(line, ctx->output);
        }
        return;
    }
    
    char included_filename[MAX_FILENAME_IN_DIRECTIVE];
    if (!extract_quoted_filename(line, included_filename)) {
        fprintf(stderr, "Error: Invalid #include directive at line %d\n", ctx->current_line);
        return;
    }
    
    char resolved_path[MAX_PATH_LENGTH];
    resolve_include_path(ctx->input_filename, included_filename, resolved_path);
    
    if (is_file_already_processed(resolved_path)) {
        return;
    }
    
    mark_file_as_processed(resolved_path);
    
    insert_file_content_with_includes(ctx, resolved_path);
}
