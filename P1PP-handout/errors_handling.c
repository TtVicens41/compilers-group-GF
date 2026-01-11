#include "errors_handling.h"
#include <string.h>

void init_error(
    const char *message,
    int line
) {
    strcpy(errors.message, message);
    errors.line = line;
}
