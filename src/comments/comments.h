/**
 * @title: Comments Processing.
 * @authors: Davi Penna-Mattos, Pol Goicoechea Esparza, Marc Bosch Manzano.
 * @creation: before 2026/01/27
 */

#ifndef COMMENTS_H
#define COMMENTS_H

#include <stdbool.h>

/**
 * Removes all the comments in a line within the C syntax.
 * 
 * Comments can be:
 * 
 *  - inline: marked with double bars.
 * 
 *  - block: opened by bar asterisk and closed by asterisk bar. 
 * 
 * @param line A string of a file text line.
 * @param in_block_comment Tells if the current line belongs to the a block
 * comment.
 */
void remove_comments(char *line, bool *in_block_comment);

#endif
