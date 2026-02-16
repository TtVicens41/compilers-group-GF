/**
 * @title: parse_arguments.c
 * @authors: Joan Vicente, Pau Puig
 * @creation: 16/02/2025
 */


#include "./parse_arguments.h"
#include "../language_defs.h"
#include "../utils/utils.h"

char *parse_arguments(int argc, char *argv[]) {
    if (argc <= 1 || !argv || !argv[1]) {
        return NULL;
    }
    return get_copy(argv[1]);
}
