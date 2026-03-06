/**
 * @file grammar.c
 * @brief Query helpers for Language data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar.h"

/* ── look-ups ───────────────────────────────────────────────────────── */

const char *grammar_symbol_name(const Language *lang, int symbol_id)
{
    if (!lang) return "?";
    for (int i = 0; i < lang->symbol_count; i++) {
        if (lang->symbols[i].id == symbol_id)
            return lang->symbols[i].name;
    }
    return "?";
}

SymbolType grammar_symbol_type(const Language *lang, int symbol_id)
{
    if (!lang) return SYMBOL_TERMINAL;
    for (int i = 0; i < lang->symbol_count; i++) {
        if (lang->symbols[i].id == symbol_id)
            return lang->symbols[i].type;
    }
    return SYMBOL_TERMINAL;
}

const Production *grammar_get_production(const Language *lang, int prod_id)
{
    if (!lang) return NULL;
    for (int i = 0; i < lang->production_count; i++) {
        if (lang->productions[i].id == prod_id)
            return &lang->productions[i];
    }
    return NULL;
}

int grammar_find_symbol_by_name(const Language *lang, const char *name)
{
    if (!lang || !name) return -1;
    for (int i = 0; i < lang->symbol_count; i++) {
        if (strcmp(lang->symbols[i].name, name) == 0)
            return lang->symbols[i].id;
    }
    return -1;
}

/* ── formatting ─────────────────────────────────────────────────────── */

char *grammar_format_production(const Language *lang, int prod_id)
{
    const Production *p = grammar_get_production(lang, prod_id);
    if (!p) return NULL;

    /* worst-case: lhs(31) + " -> " + 10 * (name(31) + ' ') + '\0' */
    int cap = 400;
    char *buf = malloc((size_t)cap);
    if (!buf) return NULL;

    int pos = 0;
    pos += snprintf(buf + pos, (size_t)(cap - pos), "%s ->",
                    grammar_symbol_name(lang, p->lhs));

    for (int i = 0; i < p->rhs_len && pos < cap; i++) {
        pos += snprintf(buf + pos, (size_t)(cap - pos), " %s",
                        grammar_symbol_name(lang, p->rhs[i]));
    }
    return buf;
}

/* ── counting ───────────────────────────────────────────────────────── */

int grammar_terminal_count(const Language *lang)
{
    int count = 0;
    if (!lang) return 0;
    for (int i = 0; i < lang->symbol_count; i++) {
        if (lang->symbols[i].type == SYMBOL_TERMINAL)
            count++;
    }
    return count;
}

int grammar_nonterminal_count(const Language *lang)
{
    int count = 0;
    if (!lang) return 0;
    for (int i = 0; i < lang->symbol_count; i++) {
        if (lang->symbols[i].type == SYMBOL_NONTERMINAL)
            count++;
    }
    return count;
}
