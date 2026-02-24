/**
 * @file dfa.h
 * @brief Deterministic finite automaton wrapper around the parse table.
 *
 * The DFA tracks the current state and delegates look-ups to the
 * ParseTable.  It knows nothing about the parser stack.
 */

#ifndef DFA_H
#define DFA_H

#include "parse_table.h"

typedef struct {
    ParseTable *table;
    int current_state;
} DFA;

/** Allocate a DFA that wraps the given table (does NOT own it). */
DFA *dfa_init(ParseTable *table);

/** Query ACTION(current_state, terminal_id). */
Action dfa_get_action(const DFA *dfa, int terminal_id);

/** Query GOTO(current_state, nonterminal_id). */
int dfa_get_goto(const DFA *dfa, int nonterminal_id);

/** Explicitly set the current state. */
void dfa_set_state(DFA *dfa, int state);

/** Free the DFA struct (does NOT free the underlying ParseTable). */
void dfa_destroy(DFA *dfa);

#endif /* DFA_H */
