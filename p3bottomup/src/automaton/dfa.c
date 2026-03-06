/**
 * @file dfa.c
 * @brief DFA – thin wrapper around the parse table.
 *
 * Tracks the current state and forwards look-ups to the ParseTable.
 * Has no knowledge of the parser stack.
 * 
 * @author Davi
 */

#include <stdlib.h>

#include "dfa.h"
#include "../utils/string_utils.h"

DFA *dfa_init(ParseTable *table)
{
    if (!table) return NULL;

    DFA *dfa = malloc(sizeof(DFA));
    if (!dfa) return NULL;

    dfa->table         = table;
    dfa->current_state = 0;
    return dfa;
}

Action dfa_get_action(const DFA *dfa, int terminal_id)
{
    return parse_table_get_action(dfa->table, dfa->current_state,
                                  terminal_id);
}

int dfa_get_goto(const DFA *dfa, int nonterminal_id)
{
    return parse_table_get_goto(dfa->table, dfa->current_state,
                                nonterminal_id);
}

void dfa_set_state(DFA *dfa, int state)
{
    if (dfa) dfa->current_state = state;
}

void dfa_destroy(DFA *dfa)
{
    free(dfa); 
}

char *dfa_string(const DFA *dfa, int level) {
    if (!dfa) {
        return NULL;
    }
    char *s = get_copy("\n");
    int n = level + 1;
    jsonify_wrap(&s, level, 1, "{");
    jsonify(&s, n, 1, 1, "parse_table",   parse_table_string(dfa->table, n));
    jsonify(&s, n, 0, 1, "current_state", int_to_str(dfa->current_state));
    jsonify_wrap(&s, level, 0, "}");
    return s;
}