#ifndef INCLUDE_H
#define INCLUDE_H

#include "../context.h"

#define MAX_PATH_LENGTH 512
#define MAX_INCLUDED_FILES 128
#define MAX_FILENAME_IN_DIRECTIVE 256

void handle_include(PreprocessorContext *ctx, const char *line);

#endif
