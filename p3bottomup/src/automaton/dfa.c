/**
 * @file dfa.c
 * @brief DFA – thin wrapper around the parse table.
 *
 * Tracks the current state and forwards look-ups to the ParseTable.
 * Has no knowledge of the parser stack.
 */

#include <stdlib.h>

#include "dfa.h"

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
    free(dfa);   /* does NOT free the underlying ParseTable */
}
