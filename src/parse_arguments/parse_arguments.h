#ifndef PARSE_ARGUMENTS_H
#define PARSE_ARGUMENTS_H

#include "context.h"
#include <stdio.h>

#define FLAG_HELP "-help"
#define FLAG_COMMENTS "-c"
#define FLAG_DIRECTIVES "-d"
#define FLAG_ALL "-all"

void parse_arguments(int argc, char *argv[], PreprocessorContext *ctx);

void print_file(const char *path);

#endif