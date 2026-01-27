#ifndef PARSE_ARGUMENTS_H
#define PARSE_ARGUMENTS_H

#include "context.h"
#include <stdio.h>

void parse_arguments(int argc, char *argv[], PreprocessorContext *ctx);

void print_file(const char *path);


#endif