/**
 * @title: Keyword Dispatcher.
 * @brief: To do handles of keywords. It is a general file, reusable for
 * other modules.
 * @authors: Davi Panna-Mattos Dias de Paiva.
 * @creation: before 2026/01/27.
 */

#ifndef KEYWORD_DISPATCHER_H
#define KEYWORD_DISPATCHER_H

#include <stdbool.h>
#include "context.h"

/*
 * WordHandler
 * -----------
 * Function pointer type for handling a specific word.
 * Takes the preprocessor context and the line containing the word.
 */
typedef void (*KeywordHandler)(PreprocessorContext *ctx, const char *line);

/*
 * KeywordHandlerPair
 * ---------------
 * Maps a keyword to its handler function.
 * Used to build lookup tables for word dispatching.
 */
typedef struct {
    const char *keyword;
    KeywordHandler handler;
} KeywordHandlerPair;

/*
 * find_and_handle_keyword
 * -----------------------
 * Dispatches a keyword to its handler by looking it up in the table.
 * 
 * Parameters:
 *   - ctx: preprocessor context
 *   - keyword: the keyword to match against the table
 *   - table: array of KeywordHandlerPair, terminated by {NULL, NULL}
 *   - line: the full line containing the keyword (passed to handler)
 * 
 * Returns:
 *   true if the keyword was found in the table and handled, false otherwise.
 */
bool find_and_handle_keyword(PreprocessorContext *ctx, const char *keyword, 
                          const KeywordHandlerPair *table, const char *line);

/*
 * extract_first_keyword
 * ------------------
 * Extracts the first keyword from a string, skipping leading whitespace.
 * 
 * Parameters:
 *   - str: input string
 *   - buffer: output buffer to store the keyword
 *   - buffer_size: maximum size of the buffer
 * 
 * Returns:
 *   pointer to the first character after the extracted keyword in str,
 *   or NULL if no keyword was found.
 */
const char *extract_first_keyword(const char *str, char *buffer, int buffer_size);

#endif