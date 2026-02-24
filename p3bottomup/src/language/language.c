/**
 * @file language.c
 * @brief Loading and destruction of Language instances from file.
 *
 * File format (sections separated by blank lines or section headers):
 *
 *   SYMBOLS
 *   T <id> <name>      – terminal
 *   N <id> <name>      – nonterminal
 *   ...
 *
 *   PRODUCTIONS
 *   <prod_id> <lhs_id> <rhs_len> <rhs_id_0> ... <rhs_id_n>
 *   ...
 *
 *   START
 *   <symbol_id>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "language.h"

/* ── helpers ────────────────────────────────────────────────────────── */

/** Trim leading/trailing whitespace in place and return the pointer. */
static char *trim(char *s)
{
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') s++;
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        *end-- = '\0';
    return s;
}

static int is_blank_or_empty(const char *line)
{
    while (*line) {
        if (*line != ' ' && *line != '\t' && *line != '\r' && *line != '\n')
            return 0;
        line++;
    }
    return 1;
}

/* ── line-level parsers ──────────────────────────────────────────────── */

static int parse_symbol_line(const char *line, Symbol **syms,
                             int *count, int *cap)
{
    char type_ch;
    int id;
    char name[32];
    if (sscanf(line, "%c %d %31s", &type_ch, &id, name) != 3) return -1;

    if (*count >= *cap) {
        *cap *= 2;
        *syms = realloc(*syms, sizeof(Symbol) * (size_t)*cap);
        if (!*syms) return -1;
    }
    (*syms)[*count].id   = id;
    (*syms)[*count].type = (type_ch == 'N') ? SYMBOL_NONTERMINAL
                                            : SYMBOL_TERMINAL;
    strncpy((*syms)[*count].name, name,
            sizeof((*syms)[*count].name) - 1);
    (*syms)[*count].name[sizeof((*syms)[*count].name) - 1] = '\0';
    (*count)++;
    return 0;
}

static int parse_production_line(const char *line, Production **prods,
                                 int *count, int *cap)
{
    int id, lhs, rhs_len;
    int pos = 0, consumed = 0;
    if (sscanf(line + pos, "%d %d %d%n", &id, &lhs, &rhs_len, &consumed) < 3)
        return -1;
    pos += consumed;

    if (*count >= *cap) {
        *cap *= 2;
        *prods = realloc(*prods, sizeof(Production) * (size_t)*cap);
        if (!*prods) return -1;
    }
    (*prods)[*count].id      = id;
    (*prods)[*count].lhs     = lhs;
    (*prods)[*count].rhs_len = rhs_len;

    for (int i = 0; i < rhs_len && i < 10; i++) {
        int sym;
        if (sscanf(line + pos, "%d%n", &sym, &consumed) < 1) sym = -1;
        pos += consumed;
        (*prods)[*count].rhs[i] = sym;
    }
    (*count)++;
    return 0;
}

/* ── public API ─────────────────────────────────────────────────────── */

enum { SEC_NONE, SEC_SYMBOLS, SEC_PRODUCTIONS, SEC_START };

Language *language_load_from_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "language: cannot open '%s'\n", path);
        return NULL;
    }

    Language *lang = calloc(1, sizeof(Language));
    if (!lang) { fclose(fp); return NULL; }

    int sym_cap  = 16, prod_cap = 16;
    lang->symbols     = malloc(sizeof(Symbol)     * (size_t)sym_cap);
    lang->productions = malloc(sizeof(Production) * (size_t)prod_cap);
    if (!lang->symbols || !lang->productions) goto fail;

    int section = SEC_NONE;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        char *t = trim(line);
        if (is_blank_or_empty(t)) continue;
        if (t[0] == '#') continue;     /* comment */

        /* section headers */
        if (strcmp(t, "SYMBOLS")     == 0) { section = SEC_SYMBOLS;     continue; }
        if (strcmp(t, "PRODUCTIONS") == 0) { section = SEC_PRODUCTIONS; continue; }
        if (strcmp(t, "START")       == 0) { section = SEC_START;       continue; }

        switch (section) {
        case SEC_SYMBOLS:
            parse_symbol_line(t, &lang->symbols,
                              &lang->symbol_count, &sym_cap);
            break;
        case SEC_PRODUCTIONS:
            parse_production_line(t, &lang->productions,
                                  &lang->production_count, &prod_cap);
            break;
        case SEC_START:
            sscanf(t, "%d", &lang->start_symbol);
            section = SEC_NONE;
            break;
        default:
            break;
        }
    }

    fclose(fp);
    return lang;

fail:
    fclose(fp);
    language_destroy(lang);
    return NULL;
}

void language_destroy(Language *lang)
{
    if (!lang) return;
    free(lang->symbols);
    free(lang->productions);
    free(lang);
}
