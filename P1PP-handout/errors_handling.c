#include "errors_handling.h"

void init_error(
    const char *message,
    int line
) {
    strcpy(errors.message, message);
    errors.line = line;
}
