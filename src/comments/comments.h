#ifndef COMMENTS_H
#define COMMENTS_H
#define SLASH_STAR "/*"
#define STAR_SLASH "*/"
#define DOUBLE_SLASH "//"

#include <stdbool.h>

bool remove_comments(char *line, bool *in_block_comment);

#endif
