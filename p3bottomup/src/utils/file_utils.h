/**
 * @title: File Utilities for P3 Bottom-Up Parser
 */

#ifndef P3_FILE_UTILS_H
#define P3_FILE_UTILS_H

#include <stdio.h>

FILE *open_file_or_null(const char *path, const char *mode);
char *build_output_path_with_suffix(const char *input_path, const char *suffix, const char *extension);

#endif /* P3_FILE_UTILS_H */
