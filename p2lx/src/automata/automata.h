/**
 * @title: Automata Module
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "../char_map/char_map.h"
#include "../language_defs.h"

/**
 * Deterministic Finite State Automata
 */
typedef struct {
    char *alphabet;
    int alphabet_size;   
    int states_size;  
    int initial_state;     
    int accepting_states_size;
    int *accepting_states;
    int **transitions; 
    char *char_map;
} DFA;

typedef struct {
    DFA **automatas;
    int size;
} NFA;

DFA *read_dfa(char *automaton_string);
NFA *read_union_nfa(const char *file); 
void print_nfa(const NFA *automaton);
void print_dfa(const DFA *automaton);
void print_integer_array(const int *array, int size);
void print_integer_matrix(int **matrix, int rows, int cols);
int *computational_path(const DFA *automaton, char *string);
int is_accepted_dfa(const DFA *automaton, char *string);
int is_accepted_nfa(const NFA *automaton, char *string);

#endif