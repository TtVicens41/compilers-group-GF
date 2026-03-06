/**
 * @file dfa.c
 * @brief Deterministic Finite-State Automata
 * @author Marc Bosch Manzano
 * @since 2026-02-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./dfa.h"
#include "../char_map/char_map.h"
#include "../language_defs.h"

DFA *empty_dfa(void) {
    return calloc(1, sizeof(DFA));
}

DFA *init_dfa(const char *automaton_string) {
    StringArray *automaton_string_splitted;
    automaton_string_splitted = string_split(
        automaton_string, AUTOMATA_ATTRIBUTE_SEPARATOR
    );

    const char **buffer = (const char **)automaton_string_splitted->buffer;
    const int size = (int)automaton_string_splitted->size;
    
    DFA *dfa = empty_dfa();
    if (!dfa) { 
        return NULL; 
    }
    
    dfa->states_size = size - TRANSITIONS;
    dfa->initial_state = INITIAL_STATE_NUM;
    dfa->empty_state = EMPTY_STATE_NUM;

    if (size <= CATEGORY) { 
        return dfa; 
    }

    dfa->category = get_copy(buffer[CATEGORY]);

    if (size <= ALPHABET) { 
        return dfa; 
    }

    dfa->alphabet = get_copy(buffer[ALPHABET]);
    dfa->char_map = init_char_map(buffer[ALPHABET]);
    dfa->alphabet_size = compute_num_assigned_chars(dfa->char_map);

    if (size <= ACCEPTING_STATES) { 
        return dfa; 
    }

    StringArray *accepting_states;
    accepting_states = string_split(
        buffer[ACCEPTING_STATES], AUTOMATA_LIST_SEPARATOR
    );

    dfa->accepting_states_size = accepting_states->size;
    dfa->accepting_states = to_integer_array(accepting_states, TRUE);

    if (size <= TRANSITIONS) { 
        return dfa; 
    }

    dfa->transitions_size = ASCII_SIZE;
    dfa->transitions = calloc(dfa->states_size, sizeof(int *));

    for (int j = TRANSITIONS; j < size; j++) {
        StringArray *transitions;
        transitions = string_split(buffer[j], AUTOMATA_LIST_SEPARATOR);
        
        dfa->transitions_size = min(dfa->transitions_size, transitions->size);
        dfa->transitions[j - TRANSITIONS] = to_integer_array(transitions, TRUE);
    }
    
    delete_string_array(&automaton_string_splitted);
    return dfa;
}

DFA **init_dfa_array(const StringArray *automatons_strings) {
    DFA **automatons = calloc(automatons_strings->size, sizeof(DFA *));
    if (!automatons) { 
        return NULL; 
    }
    for (int i = 0; i < automatons_strings->size; i++) {
        automatons[i] = init_dfa(automatons_strings->buffer[i]);
    }
    return automatons;
}

void reset_dfa(DFA *automaton) {
    automaton->current_state = automaton->initial_state;
}

int check_accepting_condition_dfa(const DFA *automaton) {
    for (int i = 0; i < automaton->accepting_states_size; i++) {
        if (automaton->current_state == automaton->accepting_states[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

int is_alive_dfa(const DFA *automaton) {
    return automaton->current_state != automaton->empty_state;
}

int is_look_ahead_alive_dfa(const DFA *automaton, char symbol) {
    unsigned char symbol_u = (unsigned char)symbol;
    int symbol_id = automaton->char_map[symbol_u];
    int empty_state = automaton->empty_state;
    int current_state = automaton->current_state;
    int **transitions = automaton->transitions;

    if (symbol_id == KEY_ERROR) {
        return FALSE;
    }

    if (transitions[current_state][symbol_id] == empty_state) {
        return FALSE;
    }

    return TRUE;
}

void consume_char_dfa(DFA *automaton, char symbol) {
    unsigned char symbol_u = (unsigned char)symbol;
    int symbol_id = automaton->char_map[symbol_u];
    int empty_state = automaton->empty_state;
    int current_state = automaton->current_state;
    int **transitions = automaton->transitions;

    current_state = 
        symbol_id == KEY_ERROR ?
        empty_state : transitions[current_state][symbol_id];

    automaton->current_state = current_state;
}

int accept_string_dfa(DFA *automaton, const char *string) {
    const int length = strlen(string);

    reset_dfa(automaton);
    for (int i = 0; i < length; i++) {
        consume_char_dfa(automaton, string[i]);
        if (!is_alive_dfa(automaton)) {
            return FALSE;
        }
    }

    return check_accepting_condition_dfa(automaton);
}

int is_valid_dfa(const DFA *automaton, const char *categories) {
    if (!automaton) { 
        return FALSE; 
    };

    int condition = 1;
    condition &= automaton->states_size > 0;
    condition &= automaton->alphabet_size > 0;
    condition &= automaton->accepting_states_size > 0;
    condition &= automaton->transitions_size > 0;
    condition &= automaton->category != NULL;
    condition &= automaton->alphabet != NULL;
    condition &= automaton->transitions != NULL;
    condition &= automaton->accepting_states != NULL;
    condition &= automaton->char_map != NULL;
    condition &= strstr(categories, automaton->category) != NULL;

    return condition;
}

void clear_dfa(DFA *dfa) {
    free(dfa->alphabet);
    dfa->alphabet = NULL;
    
    free(dfa->category);
    dfa->category = NULL;

    free(dfa->accepting_states);
    dfa->accepting_states = NULL;

    free(dfa->char_map);
    dfa->char_map = NULL;

    for (int i = 0; i < dfa->states_size; i++) {
        free(dfa->transitions[i]);
        dfa->transitions[i] = NULL;
    }

    free(dfa->transitions);
    dfa->transitions = NULL;

    dfa->states_size = 0;
    dfa->alphabet_size = 0;
    dfa->accepting_states_size = 0;
    dfa->transitions_size = 0;
    dfa->initial_state = 0;
    dfa->current_state = 0;
    dfa->empty_state = 0;
}

void delete_dfa(DFA **dfa) {
    if (!dfa || !*dfa) {
        return;
    }
    clear_dfa(*dfa);
    free(*dfa);
    *dfa = NULL;
}

void print_dfa(const DFA *automaton) {
    if (!automaton) { return; }

    printf("{\n");
    printf("category: %s,\n", automaton->category);
    printf("alphabet: %s,\n", automaton->alphabet);
    printf("alphabet_size: %d,\n", automaton->alphabet_size);
    printf("transitions_size: %d\n", automaton->transitions_size);
    printf("states_size: %d,\n", automaton->states_size);
    printf("initial_state: %d,\n", automaton->initial_state);
    printf("accepting_states: ");
    print_raw_integer_array(
        automaton->accepting_states, 
        automaton->accepting_states_size
    );
    printf(
        ",\naccepting_states_size: %d,\n", 
        automaton->accepting_states_size
    );
    printf("transitions: ");
    print_raw_integer_matrix(
        automaton->transitions,
        automaton->states_size,
        automaton->transitions_size
    );
    printf("}\n");
}
