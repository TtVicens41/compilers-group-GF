#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "../char_map/char_map.h"
#include "../token.h"

#define EMPTY_STATE 0

typedef struct {
    TokenCategory category;
    char *alphabet;
    int alphabet_size;
    int states_size;
    int initial_state;
    int *accepting_states;
    int accepting_states_size;
    int **transitions;
    char *char_map;
} DFA;

typedef struct {
    DFA **automatas;
    int size;
} NFA;

DFA *read_dfa(char *automaton_string);
NFA *read_union_nfa(const char *file);

int is_accepted_dfa(const DFA *automaton, const char *string);
TokenCategory classify_lexeme_nfa(const NFA *automaton, const char *string);

void free_dfa(DFA *automaton);
void free_nfa(NFA *automaton);

void print_dfa(const DFA *automaton);
void print_nfa(const NFA *automaton);

#endif
