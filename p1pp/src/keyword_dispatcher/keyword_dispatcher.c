/**
 * @title: Keyword Dispatcher.
 * @brief: To do handles of keywords. It is a general file, reusable for
 * other modules.
 * @authors: Davi Panna-Mattos Dias de Paiva.
 * @creation: before 2026/01/27.
 */

#include <string.h>
#include <ctype.h>

#include "keyword_dispatcher.h"

bool find_and_handle_keyword(PreprocessorContext *ctx, const char *keyword, 
                          const KeywordHandlerPair *table, const char *line) {
    if (!keyword || !table) {
        return false;
    }

    // Search through the table for a matching keyword
    for (int table_index = 0; table[table_index].keyword != NULL; table_index++) {
        if (strcmp(keyword, table[table_index].keyword) == 0) {
            // Found a match, call the handler
            if (table[table_index].handler) {
                table[table_index].handler(ctx, line);
            }
            return true;
        }
    }

    // Keyword not found in the table
    return false;
}

const char *extract_first_keyword(const char *str, char *buffer, int buffer_size) {
    if (!str || !buffer || buffer_size <= 0) {
        return NULL;
    }

    const char *current_position = str;
    
    // Skip leading whitespace
    while (isspace(*current_position)) {
        current_position++;
    }

    // If we reached the end, no word found
    if (*current_position == '\0') {
        buffer[0] = '\0';
        return NULL;
    }

    // Extract the word
    int buffer_index = 0;
    while (*current_position && !isspace(*current_position) && buffer_index < buffer_size - 1) {
        buffer[buffer_index++] = *current_position++;
    }
    buffer[buffer_index] = '\0';

    // Return pointer to the rest of the string
    return current_position;
}

