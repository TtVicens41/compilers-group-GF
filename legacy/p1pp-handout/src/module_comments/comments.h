/**
 * Comments Processor Module
 * @author: Marc Bosch Manzano & Pol Goicoechea Esparza
 * @creation: 09/01/2026
 * @revisions: (14/01/2026) by authors. Added documentation.
 */

#ifndef COMMENTS_H
#define COMMENTS_H

/**
 * Copy a file replacing c comments by newline characters.
 * @param input_path Origin file path to be read.
 * @param output_path Destination file path to be writen.
 */
void delete_comments(
    const char *input_path, 
    const char *output_path
);

/**
 * Copy a file deleting empty newlines.
 * @param input_path Origin file path to be read.
 * @param output_path Destination file path to be writen.
 */
void delete_empty_rows(
    const char *input_path, 
    const char *output_path
);

/**
 * Copy a file deleting c comments and empty newlines.
 * @param input_path Origin file path to be read.
 * @param output_path Destination file path to be writen.
 */
void process_comments(
    const char *input_path, 
    const char *output_path
);

#endif