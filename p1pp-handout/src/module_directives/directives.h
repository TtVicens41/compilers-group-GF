/**
 * @title: Directives Processor Module
 * @brief: Handles the -d flag to process preprocessor directives.
 *      Removes #define, #include, #ifndef, #endif and other directives,
 *      replacing defined constants and macros with their values.
 * @creation: 11/01/2026
 * @authors: Pau Puig Guillén
 * @revisions: (14/01/2026). Marc Bosch Manzano & Pol Goicoechea Esparza. 
 *      Reviewed file documentation.
 */

#ifndef DIRECTIVES_H
#define DIRECTIVES_H

/**
 * Process the command that replaces c directives (#include, #define, #ifdef, 
 * #endif, etc.) to its text content.
 * @param input_path Origin file path to be read.
 * @param output_path Destination file path to be writen.
 */
void process_directives(
    const char *input_path, 
    const char *output_path
);

#endif
