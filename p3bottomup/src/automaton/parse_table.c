/**
 * @file parse_table.c
 * @brief Loading and querying of the ACTION / GOTO parse table.
 *
 * File format:
 *
 *   STATES  <n>
 *   TERMINALS  <n>
 *   NONTERMINALS  <n>
 *
 *   ACTION
 *   <state> <terminal_id> <S|R|A> <value>
 *   ...
 *
 *   GOTO
 *   <state> <nonterminal_id> <target_state>
 *   ...
 *
 * Entries not listed default to ACTION_ERROR / -1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_table.h"
#include "../utils/string_utils.h"

/* ── allocation helpers ─────────────────────────────────────────────── */

static Action **alloc_action_table(int states, int terminals)
{
    Action **tbl = malloc(sizeof(Action *) * (size_t)states);
    if (!tbl) return NULL;

    for (int i = 0; i < states; i++) {
        tbl[i] = malloc(sizeof(Action) * (size_t)terminals);
        if (!tbl[i]) return NULL;  /* simplified error handling */
        for (int j = 0; j < terminals; j++) {
            tbl[i][j].type  = ACTION_ERROR;
            tbl[i][j].value = 0;
        }
    }
    return tbl;
}

static int **alloc_goto_table(int states, int nonterminals)
{
    int **tbl = malloc(sizeof(int *) * (size_t)states);
    if (!tbl) return NULL;

    for (int i = 0; i < states; i++) {
        tbl[i] = malloc(sizeof(int) * (size_t)nonterminals);
        if (!tbl[i]) return NULL;
        for (int j = 0; j < nonterminals; j++) {
            tbl[i][j] = -1;
        }
    }
    return tbl;
}

/* ── section parsers ────────────────────────────────────────────────── */

static int parse_header(FILE *fp, int *states, int *terms, int *nonterms)
{
    char line[256];
    int found = 0;

    while (found < 3 && fgets(line, sizeof(line), fp)) {
        char *t = trim(line);
        if (is_blank(t)) continue;
        if (sscanf(t, "STATES %d",       states)   == 1) { found++; continue; }
        if (sscanf(t, "TERMINALS %d",    terms)     == 1) { found++; continue; }
        if (sscanf(t, "NONTERMINALS %d", nonterms)  == 1) { found++; continue; }
        if (strcmp(t, "ACTION") == 0) break;
    }
    return (found == 3) ? 0 : -1;
}

static int parse_action_section(FILE *fp, ParseTable *pt)
{
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        char *t = trim(line);
        if (is_blank(t)) continue;
        if (strcmp(t, "GOTO") == 0) break;
        if (strcmp(t, "ACTION") == 0) continue;

        int state, tid, val = 0;
        char type_ch;
        if (sscanf(t, "%d %d %c %d", &state, &tid, &type_ch, &val) < 3)
            continue;

        if (state < 0 || state >= pt->state_count) continue;
        if (tid < 0 || tid >= pt->terminal_count)  continue;

        switch (type_ch) {
        case 'S': pt->action_table[state][tid].type = ACTION_SHIFT;  break;
        case 'R': pt->action_table[state][tid].type = ACTION_REDUCE; break;
        case 'A': pt->action_table[state][tid].type = ACTION_ACCEPT; break;
        default:  pt->action_table[state][tid].type = ACTION_ERROR;  break;
        }
        pt->action_table[state][tid].value = val;
    }
    return 0;
}

static int parse_goto_section(FILE *fp, ParseTable *pt)
{
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        char *t = trim(line);
        if (is_blank(t)) continue;

        int state, nt_id, target;
        if (sscanf(t, "%d %d %d", &state, &nt_id, &target) != 3)
            continue;

        int offset = nt_id - pt->terminal_count;
        if (state < 0  || state >= pt->state_count)      continue;
        if (offset < 0 || offset >= pt->nonterminal_count) continue;

        pt->goto_table[state][offset] = target;
    }
    return 0;
}

/* ── public API ─────────────────────────────────────────────────────── */

ParseTable *parse_table_load_from_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "parse_table: cannot open '%s'\n", path);
        return NULL;
    }

    int states = 0, terms = 0, nonterms = 0;
    if (parse_header(fp, &states, &terms, &nonterms) < 0) {
        fprintf(stderr, "parse_table: invalid header in '%s'\n", path);
        fclose(fp);
        return NULL;
    }

    ParseTable *pt = calloc(1, sizeof(ParseTable));
    if (!pt) { fclose(fp); return NULL; }

    pt->state_count       = states;
    pt->terminal_count    = terms;
    pt->nonterminal_count = nonterms;
    pt->action_table      = alloc_action_table(states, terms);
    pt->goto_table        = alloc_goto_table(states, nonterms);

    if (!pt->action_table || !pt->goto_table) {
        parse_table_destroy(pt);
        fclose(fp);
        return NULL;
    }

    /* The file pointer is now somewhere after the header.  We may have
       already consumed the ACTION marker inside parse_header, or it may
       come next.  Re-scan from here. */
    parse_action_section(fp, pt);
    parse_goto_section(fp, pt);

    fclose(fp);
    return pt;
}

Action parse_table_get_action(const ParseTable *table, int state,
                              int terminal_id)
{
    Action err = { ACTION_ERROR, 0 };
    if (!table) return err;
    if (state < 0 || state >= table->state_count)       return err;
    if (terminal_id < 0 || terminal_id >= table->terminal_count) return err;
    return table->action_table[state][terminal_id];
}

int parse_table_get_goto(const ParseTable *table, int state,
                         int nonterminal_id)
{
    if (!table) return -1;
    int offset = nonterminal_id - table->terminal_count;
    if (state < 0  || state >= table->state_count)       return -1;
    if (offset < 0 || offset >= table->nonterminal_count) return -1;
    return table->goto_table[state][offset];
}

void parse_table_destroy(ParseTable *table)
{
    if (!table) return;
    if (table->action_table) {
        for (int i = 0; i < table->state_count; i++)
            free(table->action_table[i]);
        free(table->action_table);
    }
    if (table->goto_table) {
        for (int i = 0; i < table->state_count; i++)
            free(table->goto_table[i]);
        free(table->goto_table);
    }
    free(table);
}

char *parse_table_string(const ParseTable *table, int level) 
{
    if (!table) {
        return NULL;
    }
    char *s = get_copy("\n");
    const int n = level + 1;
    char *action_table = action_table_string(
        table->action_table,
        table->state_count,
        table->terminal_count, 
        n
    );
    char *goto_table = int_matrix_to_str(
        table->goto_table, 
        table->state_count, 
        table->nonterminal_count, 
        n
    );
    jsonify_wrap(&s, level, 1, "{");
    jsonify(&s, n, 1, 1, "action_table",      action_table);
    jsonify(&s, n, 1, 1, "goto_table",        goto_table);
    jsonify(&s, n, 1, 1, "state_count",       int_to_str(table->state_count));
    jsonify(&s, n, 1, 1, "terminal_count",    int_to_str(table->terminal_count));
    jsonify(&s, n, 0, 1, "nonterminal_count", int_to_str(table->nonterminal_count));
    jsonify_wrap(&s, level, 0, "}");
    return s;
}