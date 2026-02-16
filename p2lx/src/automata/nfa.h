/**
 * DFA-Union Non-Deterministic Finite-State Automata
 * 
 * @description:
 *      Particular case of a NFA defined as union of DFAs. Its implementation
 *      consists as a list of DFAs. The intial state of the NFA has epsilon 
 *      transitions to all initial states of DFA automatas. That can be 
 *      considered as running all DFA automatas and observe wether any automata
 *      accepts the imput string.
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/13
 */

#ifndef UNION_NFA_H
#define UNION_NFA_H

#include "./dfa.h"

/**
 * DFA-Union Non-Deterministic Finite State Automata
 */
typedef struct {
    DFA **automatons;
    int size;
} NFA; 

NFA *init_union_nfa(const char *automatons_string); 
void clear_nfa(NFA *automaton);
void delete_nfa(NFA **automaton);
void reset_nfa(NFA *automaton);
void consume_char_nfa(NFA *automaton, char symbol);
int accept_string_nfa(NFA *automaton, const char *string);
int is_valid_nfa(const NFA *automaton, const char *categories);
int is_alive_nfa(const NFA *automaton);
int is_look_ahead_alive_nfa(const NFA *automaton, char symbol);
void print_nfa(const NFA *automaton);

#endif // UNION_NFA_H
