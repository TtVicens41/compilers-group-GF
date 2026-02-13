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
 * @brief Explica la responsabilidad de `is_accepted_dfa` en el flujo del compilador.
 * @param Recibe: `const DFA *automaton, const char *string`.
 * @return Devuelve un valor de tipo `int`.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
int is_accepted_dfa(const DFA *automaton, const char *string);
/**
 * @brief Explica la responsabilidad de `classify_lexeme_nfa` en el flujo del compilador.
 * @param Recibe: `const NFA *automaton, const char *string`.
 * @return Devuelve un valor de tipo `TokenCategory`.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
TokenCategory classify_lexeme_nfa(const NFA *automaton, const char *string);

/**
 * @brief Explica la responsabilidad de `free_dfa` en el flujo del compilador.
 * @param Recibe: `DFA *automaton`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void free_dfa(DFA *automaton);
/**
 * @brief Explica la responsabilidad de `free_nfa` en el flujo del compilador.
 * @param Recibe: `NFA *automaton`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void free_nfa(NFA *automaton);

/**
 * @brief Explica la responsabilidad de `print_dfa` en el flujo del compilador.
 * @param Recibe: `const DFA *automaton`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void print_dfa(const DFA *automaton);
/**
 * @brief Explica la responsabilidad de `print_nfa` en el flujo del compilador.
 * @param Recibe: `const NFA *automaton`.
 * @return No devuelve valor.
 * @details Ejecuta una tarea concreta para mantener el codigo modular y facilitar mantenimiento.
 */
void print_nfa(const NFA *automaton);

#endif
