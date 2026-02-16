/**
 * Argumennts Menu.
 * 
 * @description: Implementations of arguments parsing utilities.
 * @authors: Marc Bosch Manzano
 * @creation: 2026/01/15.
 * @see_also: Adapted from P1 Preprocessor.
 */

#include "./parse_arguments.h"
#include "../language_defs.h"
#include "../utils/utils.h"

char *parse_arguments(int argc, char *argv[]) {
    if (argc <= 1) {
        return NULL;
    }
    return get_copy(argv[1]);
}
