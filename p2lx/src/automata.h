/**
 * @title: Automata Module
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "hash_map.h"

/**
 * Deterministic Finite State Automata
 */
typedef struct {
    int alphabet_size;   
    int states_size;  
    int initial_state;     
    int *accepting_states; 
    int **transitions; 
    char *char_map;
} DFA;


#endif