#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "../language_defs.h"

void print_file(const char *path);
char *read_file(const char *path);
void dump_file(const char *path, char *str, int max_len);
void copy_file(const char *input_path, const char *output_path);
int check_input_file(const char *input_path);
int check_output_file(const char *output_path);

#endif
