/**
 * @file token_loader.c
 * @brief Load a TokenList from a .cscn scan file.
 *
 * Expected file format – one token per line:
 *
 *     TERMINAL_NAME LEXEME
 *
 * Lines starting with '#' are comments and are ignored.
 * Blank lines are ignored.
 * The terminal name is resolved against the Language symbol table.
 * The last meaningful token should normally be the end-of-input marker "$".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token_loader.h"
#include "../language/grammar.h"

/* ── helpers ────────────────────────────────────────────────────────── */

static char *trim(char *s)
{
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') s++;
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        *end-- = '\0';
    return s;
}

static int is_blank_or_comment(const char *line)
{
    while (*line == ' ' || *line == '\t') line++;
    return (*line == '\0' || *line == '\n' || *line == '#');
}

/* ── public API ─────────────────────────────────────────────────────── */

TokenList *token_loader_load_from_file(const char *path,
                                       const Language *lang)
{
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "token_loader: cannot open '%s'\n", path);
        return NULL;
    }

    TokenList *list = token_list_create();
    if (!list) { fclose(fp); return NULL; }

    char line[256];
    int line_no = 0;

    while (fgets(line, sizeof(line), fp)) {
        line_no++;
        if (is_blank_or_comment(line)) continue;

        char *trimmed = trim(line);

        /* split into TERMINAL_NAME and LEXEME */
        char name[64];
        char lexeme[64];
        int n = sscanf(trimmed, "%63s %63s", name, lexeme);

        if (n < 1) continue;
        if (n == 1) {
            /* name only – use name as lexeme too (e.g. "$") */
            strncpy(lexeme, name, sizeof(lexeme) - 1);
            lexeme[sizeof(lexeme) - 1] = '\0';
        }

        int id = grammar_find_symbol_by_name(lang, name);
        if (id < 0) {
            fprintf(stderr, "token_loader: line %d: unknown terminal '%s'\n",
                    line_no, name);
            token_list_destroy(list);
            fclose(fp);
            return NULL;
        }

        token_list_add(list, id, lexeme);
    }

    fclose(fp);
    return list;
}
