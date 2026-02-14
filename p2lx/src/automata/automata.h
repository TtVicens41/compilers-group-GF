/**
 * @title: automata.h
 * @authors:
 * @creation:
 */

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

/**
 * @brief Describes the responsibility of `is_accepted_dfa` in the compiler pipeline.
 * @param Receives: `const DFA *automaton, const char *string`.
 * @return Returns a value of type `int`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
int is_accepted_dfa(const DFA *automaton, const char *string);
/**
 * @brief Describes the responsibility of `classify_lexeme_nfa` in the compiler pipeline.
 * @param Receives: `const NFA *automaton, const char *string`.
 * @return Returns a value of type `TokenCategory`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
TokenCategory classify_lexeme_nfa(const NFA *automaton, const char *string);

/**
 * @brief Describes the responsibility of `free_dfa` in the compiler pipeline.
 * @param Receives: `DFA *automaton`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void free_dfa(DFA *automaton);
/**
 * @brief Describes the responsibility of `free_nfa` in the compiler pipeline.
 * @param Receives: `NFA *automaton`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void free_nfa(NFA *automaton);

/**
 * @brief Describes the responsibility of `print_dfa` in the compiler pipeline.
 * @param Receives: `const DFA *automaton`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void print_dfa(const DFA *automaton);
/**
 * @brief Describes the responsibility of `print_nfa` in the compiler pipeline.
 * @param Receives: `const NFA *automaton`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void print_nfa(const NFA *automaton);

#endif
