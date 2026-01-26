#include "directives.h"
#include <string.h>
#include <ctype.h>

static void handle_unsupported(PreprocessorContext *ctx, const char *line) {
    // For now: copy directive as-is or ignore, but do NOT crash
}

static DirectiveEntry directive_table[] = {
    {"define", handle_unsupported},
    {"include", handle_unsupported},
    {"ifdef", handle_unsupported},
    {"endif", handle_unsupported},
    {NULL, NULL}
};

bool process_directive(PreprocessorContext *ctx, const char *line) {
    const char *p = line;

    while (isspace(*p)) p++;
    if (*p != '#') return false;

    p++; // skip '#'
    while (isspace(*p)) p++;

    char keyword[32];
    int i = 0;
    while (*p && !isspace(*p) && i < 31) {
        keyword[i++] = *p++;
    }
    keyword[i] = '\0';

    for (int j = 0; directive_table[j].keyword; j++) {
        if (strcmp(keyword, directive_table[j].keyword) == 0) {
            directive_table[j].handler(ctx, line);
            return true;
        }
    }

    handle_unsupported(ctx, line);
    return true;
}
