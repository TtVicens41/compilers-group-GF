/**
 * @title: nfa.h
 * @authors: Joan Vicente, Alejandro Poole, Marc Bosch
 * @creation: 16/02/2025
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
