/**
 * @title: Automata Module
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/file_utils.h"
#include "utils/string_list.h"
#include "utils/string_utils.h"
#include "automata.h"

DFA *empty_dfa(void) {
    return calloc(1, sizeof(DFA));
}

NFA *empty_nfa(void) {
    return calloc(1, sizeof(NFA));
}

DFA *read_dfa(char *automaton_string) {
    StringList *string_splitted = string_split(
        automaton_string, AUTOMATA_ATTR_SEPARATOR
    );
    char **buffer = string_splitted->buffer;
    int lines = string_splitted->size;
    
    DFA *dfa = empty_dfa();
    
    dfa->alphabet = get_copy(buffer[ALPHABET]);
    dfa->alphabet_size = strlen(buffer[ALPHABET]);
    dfa->char_map = init_char_map(buffer[ALPHABET]);
    
    dfa->states_size = atoi(buffer[STATES_SIZE]);
    dfa->initial_state = atoi(buffer[INITIAL_STATE]);
    dfa->accepting_states_size = atoi(buffer[ACCEPTING_STATES_SIZE]);
    dfa->accepting_states = to_integer_array(
        string_split(buffer[ACCEPTING_STATES], AUTOMATA_LIST_SEPARATOR), 
        TRUE
    );

    dfa->transitions = calloc(dfa->states_size, sizeof(int *));
    for (int j = TRANSITIONS; j < lines; j++) {
        dfa->transitions[j - TRANSITIONS] = to_integer_array(
            string_split(buffer[j], AUTOMATA_LIST_SEPARATOR), 
            TRUE
        );
    }
    
    delete_string_list(&string_splitted);
    return dfa;
}

DFA **init_dfa_list(StringList *automata_strings) {
    DFA **automata_list = calloc(automata_strings->size, sizeof(DFA *));
    if (!automata_list) {
        return NULL; 
    }

    for (int i = 0; i < automata_strings->size; i++) {
        automata_list[i] = read_dfa(automata_strings->buffer[i]);
    }

    return automata_list;
}

NFA *read_union_nfa(const char *file) {
    char buffer[MAX_AUTOMATA_LENGTH];
    dump_file(file, buffer, MAX_AUTOMATA_LENGTH);

    StringList *automta_strings = apply_string_list(
        string_split(buffer, AUTOMATA_SEPARATOR), 
        trim_string, 
        TRUE
    );

    NFA *nfa = empty_nfa();
    nfa->size = automta_strings->size;
    nfa->automatas = init_dfa_list(automta_strings);

    delete_string_list(&automta_strings);
    return nfa;
}

int is_accepted_dfa(const DFA *automaton, char *string) {
    int length = strlen(string);

    int state = automaton->initial_state;
    for (int i = 0; i < length; i++) {
        int symbol = automaton->char_map[string[i]];
        if (symbol == KEY_ERROR) {
            return FALSE;
        }
        state = automaton->transitions[state][symbol];
    }

    for (int i = 0; i < automaton->accepting_states_size; i++) {
        if (state == automaton->accepting_states[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

int is_accepted_nfa(const NFA *automaton, char *string) {
    for (int i = 0; i < automaton->size; i++) {
        if (is_accepted_dfa(automaton->automatas[i], string)) {
            return TRUE;
        }
    }
    return FALSE;
}

void print_nfa(const NFA *automaton) {
    if (!automaton) { return; }

    printf("{\n");
    printf("dfa size: %d\n", automaton->size);
    printf("dfa list: \n");

    printf("[\n");
    for (int i = 0; i < automaton->size; i++) {
        print_dfa(automaton->automatas[i]);
    }
    printf("]\n");
    printf("}\n");
}

void print_dfa(const DFA *automaton) {
    if (!automaton) { return; }

    printf("{\n");
    printf("alphabet: %s,\n", automaton->alphabet);
    printf("alphabet_size: %d\n", automaton->alphabet_size);
    printf("states_size: %d\n", automaton->states_size );
    printf("initial_states: %d\n", automaton->initial_state);
    printf("accepting_states_size: %d\n", automaton->accepting_states_size);
    printf("accepting_states: ");
    
    print_integer_array(
        automaton->accepting_states, 
        automaton->accepting_states_size
    );

    printf("transitions: \n");

    print_integer_matrix(
        automaton->transitions,
        automaton->states_size,
        automaton->alphabet_size
    );

    printf("}\n");
}

void print_integer_matrix(int **matrix, int rows, int cols) {
    if (!matrix) { return; }

    printf("[\n");
    for (int i = 0; i < rows; i++) {
        print_integer_array(matrix[i], cols);
    }
    printf("]\n");
}

void print_integer_array(const int *array, int size) {
    if (!array) { return; }

    printf("[ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("]\n");
}