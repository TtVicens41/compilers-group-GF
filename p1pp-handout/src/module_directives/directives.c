/**
 * @title: Directives Processor Module
 * @brief: Handles the -d flag to process preprocessor directives.
 *      Removes #define, #include, #ifndef, #endif and other directives,
 *      replacing defined constants and macros with their values.
 * @creation: 11/01/2026
 * @authors: Pau Puig Guillén
 * @revisions: 
 *      (14/01/2026). Marc BM & Pol GE. Reviewed file documentation.
 *      (15/01/2026). Marc BM. Reviewed define handling.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "directives.h"
#include "../utils/string_utils.h"
#include "../utils/file_utils.h"
#include "../utils/errors_utils.h"

/// Maximum number of #define directives to store
#define MAX_DEFINES 100         

/// Structure to store a #define directive
typedef struct {
    char *name;     // Name of the constant/macro (e.g., "MY_NAME")
    char *value;    // Value to replace it with (e.g., "1000")
} Define;

/// Global table storing all #define directives found in the file
static Define defines[MAX_DEFINES];

/// Current number of defines stored
static int define_count = 0;  

/// Current skip continuanion for directives processing.
static int skip_continuation = 0;

/**
 * Add a new #define to `defines` table and increases `define_count`.
 * @param name The name of the constant/macro.
 * @param value The replacement value string.
 * @note Works whenever the table size is less than `MAX_DEFINES`.
 */
void add_define(const char *name, const char *value) {
    if (define_count >= MAX_DEFINES) {
        return;
    }
    
    defines[define_count].name = malloc(strlen(name) + 1);
    defines[define_count].value = malloc(strlen(value) + 1);
    
    strcpy(defines[define_count].name, name);
    strcpy(defines[define_count].value, value);
    
    define_count++;
}

/**
 * Search for a #define by its name from `defines` table.
 * @param name The name to search for.
 * @return The value associated with the name, or NULL if not found.
 */
const char *find_define(const char *name) {
    for (int i = 0; i < define_count; i++) {
        if (strcmp(defines[i].name, name) == 0) {
            return defines[i].value;
        }
    }
    return NULL;
}

/// Prints the `defines` table into the screen.
void print_defines() {
    printf("\n");
    for (int i = 0; i < define_count; i++) {
        printf("Define: %d\n", i + 1);
        printf("Name: %s\n", defines[i].name);
        printf("Value: %s\n\n", defines[i].value);
    }
}

/**
 * Free all dynamically allocated memory for `defines` table, and resets 
 * `define_count` to 0.
 * @note Should be called at the end of processing a file.
 */
void free_defines() {
    for (int i = 0; i < define_count; i++) {
        free(defines[i].name);
        free(defines[i].value);
    }
    define_count = 0;
}

/**
 * Replace defines in a line.
 * @param line A string representing a line in a file.
 * @param ouptut_line The processed line replacing its define whenever it exists.
 */
void replace_defines_in_line(
    char *line, 
    char *output_line
) {
    char *pos = line;
    char *out = output_line;

    while (*pos) {
        // Check if this could be an identifier
        if ((*pos >= 'A' && *pos <= 'Z') || (*pos >= 'a' && *pos <= 'z') || *pos == '_') {
            char word[256];
            int i = 0;
            
            // Extract the identifier
            while ((*pos >= 'A' && *pos <= 'Z') || 
                   (*pos >= 'a' && *pos <= 'z') || 
                   (*pos >= '0' && *pos <= '9') || 
                   *pos == '_') {
                word[i++] = *pos++;
            }
            word[i] = '\0';
            
            // Check if it's a define
            const char *value = find_define(word);
            if (value) {
                // Replace with the value
                strcpy(out, value);
                out += strlen(value);
            } else {
                // Keep the original word
                strcpy(out, word);
                out += strlen(word);
            }
        } else {
            *out++ = *pos++;
        }
    }
    *out = '\0';
}

/**
 * TODO: solve this function.
 * Sets a (`name`, `value`) pair to the `defines` table, specialized for
 * macro-instructions definitions.
 * @param define A string of a line of a file where a #define was found.
 */
void handle_define_macro(const char *define) {
    char *name = get_prefix(define, '(');
    char *value = get_copy(define + strlen(name));
    add_define(name, value);  

    // Remove trailing newline
    // char *newline = strchr(value, '\n');
    // if (newline) 
    //     *newline = '\0';
    
    // // Check for backslash continuation
    // size_t len = strlen(value);
    // if (len > 0 && value[len - 1] == '\\') {
    //     skip_continuation = 1;
    //     value[len - 1] = '\0';  // Remove the backslash
    // }
}

/**
 * Sets a (`name`, `value`) pair to the `defines` table, where `name` can be
 * either a constant or macro definition.
 * @param define A string of a line of a file where a #define was found.
 */
void handle_define(const char *define) {
    char *name = copy_until_trim(define);

    if (strchr(name, '(')) {
        handle_define_macro(define);
        free(name);
        return;
    }
    
    char *trimmed = trim(define + strlen(name));
    char *value = copy_until_trim(trimmed);
    
    add_define(name, value);   
    free(trimmed);
}

void process_directives(
    const char *input_path, 
    const char *output_path
) {
    if(!check_input_file(input_path) || !check_output_file(output_path))
        return;

    FILE *input_file = fopen(input_path, "r");
    FILE *output_file = fopen(output_path, "w");

    char line[MAX_LINE_LENGTH];
    char output_line[MAX_LINE_LENGTH * 2];
    skip_continuation = 0;
    
    while (fgets(line, sizeof(line), input_file)) {

        // Check if previous line had a continuation, i.e., ends with a backslash
        if (skip_continuation) {
            size_t len = strlen(line);
            skip_continuation = 0;
            if (len > 1 && line[len - 2] == '\\') {
                skip_continuation = 1;
            }
            continue;
        }
        
        // Handle directives (#include, #ifndef, #endif, etc.)
        char *trimmed = trim(line);
        if (trimmed[0] == '#') {
            char *directive = trim(trimmed + 1);

            if (strncmp(directive, "define", 6) == 0) {
                handle_define(directive + 7);
            }
                
            free(trimmed);
            free(directive);
            continue;
        }
        
        // Regular line or replace defines
        replace_defines_in_line(line, output_line);
        fputs(output_line, output_file);
        free(trimmed);
    }

    fclose(input_file);
    fclose(output_file);
    //print_defines();
    free_defines();
}
