/**
 * @title: dfa.h
 * @authors: Joan Vicente, Davi Paiva, Marc Bosch
 * @creation: 16/02/2025
 */


#ifndef DFA_H
#define DFA_H

#include "../utils/utils.h"

/**
 * Deterministic Finite State Automata
 */
typedef struct {
    char *category;
    char *alphabet;
    int alphabet_size;   
    int states_size;  
    int initial_state;
    int empty_state;
    int current_state;     
    int *accepting_states;
    int accepting_states_size;
    int **transitions; 
    int transitions_size;
    char *char_map;
} DFA;

DFA *init_dfa(const char *automaton_string);
DFA **init_dfa_array(const StringList *automatons_strings);
void clear_dfa(DFA *automaton);
void delete_dfa(DFA **automaton);
void reset_dfa(DFA *automaton);
void consume_char_dfa(DFA *automaton, char symbol);
int is_valid_dfa(const DFA *automaton, const char *categories);
int accept_string_dfa(DFA *automaton, const char *string);
int is_alive_dfa(const DFA *automaton);
int is_look_ahead_alive_dfa(const DFA *automaton, char symbol);
int check_accepting_condition_dfa(const DFA *automaton); 
void print_dfa(const DFA *automaton);

#endif // DFA_H
