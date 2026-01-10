#ifndef STRING_UTILS_H
#define STRING_UTILS_H

char *get_copy(const char *string);

char *get_suffix(const char *string, char separator);
char *get_suffix_r(const char *string, char separator);

char *get_prefix(const char *string, char separator);
char *get_prefix_r(const char *string, char separator);

char *add_suffix_to_path(const char *path, const char *suffix);

#endif