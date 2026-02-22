/**
 * @file parse_arguments.c
 * @brief Argumennts Menu
 * @authors Marc Bosch Manzano, Joan Vicente Martín & Pau Puig Guillén
 * @since 2026-01-15
 * @see: Adapted from P1 Preprocessor parse_arguments.c
 */

#include "./parse_arguments.h"
#include "../language_defs.h"
#include "../utils/utils.h"

#include <string.h>

char *parse_arguments(int argc, char *argv[]) {
    if (argc <= 1 || !argv || !argv[1]) {
        return NULL;
    }
    return get_copy(argv[1]);
}

void print_usage(const char *argv0) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv0);
}

char *resolve_resource_path(const char *argv0, const char *resource_name) {
    char *path;

    path = concat_strings(get_resources_dir(), resource_name);
    if (check_input_file(path)) {
        return path;
    }
    free(path);

    path = concat_strings("./p2lx/resources/", resource_name);
    if (check_input_file(path)) {
        return path;
    }
    free(path);

    if (argv0 && strchr(argv0, PATH_SEPARATOR)) {
        char *bin_dir = get_prefix_r(argv0, PATH_SEPARATOR);
        char *base = concat_strings(bin_dir, "/resources/");
        path = concat_strings(base, resource_name);
        free(base);
        free(bin_dir);
        if (check_input_file(path)) {
            return path;
        }
        free(path);
    }

    return NULL;
}

