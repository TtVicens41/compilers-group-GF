// Title: Directives Processor Module
// Brief: Handles the -d flag to process preprocessor directives
// Description: Removes #define, #include, #ifndef, #endif and other directives,
//              replacing defined constants and macros with their values
// Authors: Pau Puig
// Creation: 11/01/2026

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "directives.h"
#include "string_utils.h"
#include "errors_handling.h"

#define MAX_LINE_LENGTH 1024    // Maximum characters per line
#define MAX_DEFINES 100         // Maximum number of #define directives to store

/// @brief Structure to store a #define directive
typedef struct {
    char *name;     // Name of the constant/macro (e.g., "ON")
    char *value;    // Value to replace it with (e.g., "1")
} Define;

/// @brief Global table storing all #define directives found in the file
static Define defines[MAX_DEFINES];

/// @brief Current number of defines stored
static int define_count = 0;    

/**
 * @brief Add a new #define to the table
 * @param name The name of the constant/macro
 * @param value The replacement value
 */
void add_define(const char *name, const char *value) {
    // Check if table is full
    if (define_count >= MAX_DEFINES) {
        return;
    }
    
    // Allocate memory for the name and value strings
    defines[define_count].name = malloc(strlen(name) + 1);
    defines[define_count].value = malloc(strlen(value) + 1);
    
    // Copy the strings into the allocated memory
    strcpy(defines[define_count].name, name);
    strcpy(defines[define_count].value, value);
    
    // Increment the counter
    define_count++;
}

/**
 * Search for a #define by its name
 * @param name The name to search for
 * @return The value associated with the name, or NULL if not found
 */
const char* find_define(const char *name) {
    // Linear search through the defines table
    for (int i = 0; i < define_count; i++) {
        if (strcmp(defines[i].name, name) == 0) {
            return defines[i].value;
        }
    }
    return NULL;  // Not found
}

/**
 * Free all dynamically allocated memory for defines
 * Should be called when done processing a file
 */
void free_defines() {
    for (int i = 0; i < define_count; i++) {
        free(defines[i].name);
        free(defines[i].value);
    }
    define_count = 0;  // Reset counter
}

/**
 * Remove leading and trailing whitespace from a string
 * @param str The string to trim (modified in place)
 * @return Pointer to the trimmed string (within the original buffer)
 */
char* trim(char *str) {
    char *end;
    
    // Trim leading whitespace (spaces and tabs)
    while (*str == ' ' || *str == '\t') str++;
    
    // If string is now empty, return it
    if (*str == 0) return str;
    
    // Trim trailing whitespace (spaces, tabs, newlines, carriage returns)
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;
    
    // Null-terminate after the last non-whitespace character
    *(end + 1) = 0;
    return str;
}

// Replace defines in a line
void replace_defines_in_line(char *line, char *output_line) {
    char *pos = line;
    char *out = output_line;
    
    while (*pos) {
        // Check if this could be an identifier
        if ((*pos >= 'A' && *pos <= 'Z') || (*pos >= 'a' && *pos <= 'z') || *pos == '_') {
            char identifier[256];
            int i = 0;
            
            // Extract the identifier
            while ((*pos >= 'A' && *pos <= 'Z') || 
                   (*pos >= 'a' && *pos <= 'z') || 
                   (*pos >= '0' && *pos <= '9') || 
                   *pos == '_') {
                identifier[i++] = *pos++;
            }
            identifier[i] = '\0';
            
            // Check if it's a define
            const char *value = find_define(identifier);
            if (value) {
                // Replace with the value
                strcpy(out, value);
                out += strlen(value);
            } else {
                // Keep the original identifier
                strcpy(out, identifier);
                out += strlen(identifier);
            }
        } else {
            *out++ = *pos++;
        }
    }
    *out = '\0';
}

void process_directives(const char *input_path, const char *output_path) {
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

    char line[MAX_LINE_LENGTH];
    char output_line[MAX_LINE_LENGTH * 2];
    int skip_continuation = 0;
    
    while (fgets(line, sizeof(line), input_file)) {
        // Check if previous line had a continuation
        if (skip_continuation) {
            // Check if this line also ends with backslash
            size_t len = strlen(line);
            skip_continuation = 0;
            if (len > 1 && line[len - 2] == '\\') {
                skip_continuation = 1;
            }
            continue;
        }
        
        // Make a copy for trimming to check for directives
        char line_copy[MAX_LINE_LENGTH];
        strcpy(line_copy, line);
        char *trimmed = trim(line_copy);
        
        // Check if it's a directive
        if (trimmed[0] == '#') {
            // Skip the '#'
            char *directive = trimmed + 1;
            directive = trim(directive);
            
            // Handle #define
            if (strncmp(directive, "define", 6) == 0) {
                char *def = directive + 6;
                def = trim(def);
                
                // Extract name
                char name[256];
                int i = 0;
                while (*def && *def != ' ' && *def != '\t' && *def != '\n') {
                    name[i++] = *def++;
                }
                name[i] = '\0';
                
                // Extract value (handle multi-line defines)
                def = trim(def);
                char value[1024] = "";
                if (*def) {
                    strcpy(value, def);
                    // Remove trailing newline
                    char *newline = strchr(value, '\n');
                    if (newline) *newline = '\0';
                    
                    // Check for backslash continuation
                    size_t len = strlen(value);
                    if (len > 0 && value[len - 1] == '\\') {
                        skip_continuation = 1;
                        value[len - 1] = '\0';  // Remove the backslash
                    }
                }
                
                add_define(name, value);
                // Don't write #define lines to output
                continue;
            }
            // Handle other directives (#include, #ifndef, #endif, etc.)
            // For now, we skip them (don't write to output)
            continue;
        }
        
        // Regular line - replace defines
        replace_defines_in_line(line, output_line);
        fputs(output_line, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    free_defines();
}
