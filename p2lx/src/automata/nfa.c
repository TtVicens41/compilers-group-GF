/**
 * @title: nfa.c
 * @authors: Joan Vicente, Alejandro Poole, Marc Bosch
 * @creation: 16/02/2025
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./nfa.h"
#include "../utils/utils.h"

NFA *empty_nfa(void) {
    return calloc(1, sizeof(NFA));
}

NFA *init_union_nfa(const char *automatons_string) {
    if (!automatons_string) {
        return NULL;
    }

    NFA *nfa = empty_nfa();
    if (!nfa) { 
        return NULL; 
    }

    StringList *automata_strings;
    automata_strings = string_split(automatons_string, AUTOMATA_SEPARATOR);
    automata_strings = apply_string_list(automata_strings, trim_string, TRUE);
    
    DFA **automatons = init_dfa_array(automata_strings);
    if (!automatons) {
        return NULL;
    }

    nfa->automatons = automatons;
    nfa->size = automata_strings->size;
    
    delete_string_list(&automata_strings);
    return nfa;
}

void reset_nfa(NFA *automaton) {
    for (int i = 0; i < automaton->size; i++) {
        reset_dfa(automaton->automatons[i]);
    }
}

int is_alive_nfa(const NFA *automaton) {
    for (int i = 0; i < automaton->size; i++) {
        if (is_alive_dfa(automaton->automatons[i])) {
            return TRUE;
        }
    }

    return FALSE;
}


int is_look_ahead_alive_nfa(const NFA *automaton, char symbol) {
    int size = automaton->size;
    DFA **automatons = automaton->automatons;

    for (int i = 0; i < size; i++) {
        if (is_look_ahead_alive_dfa(automatons[i], symbol)) {
            return TRUE;
        }
    }

    return FALSE;
}

void consume_char_nfa(NFA *automaton, char symbol) {
    int size = automaton->size;
    DFA **automatons = automaton->automatons;

    for (int i = 0; i < size; i++) {
        consume_char_dfa(automatons[i], symbol);
    }
}

int is_valid_nfa(const NFA *automaton, const char *categories) {
    if (!automaton) { 
        return FALSE; 
    }
    for (int i = 0; i < automaton->size; i++) {
        if (!is_valid_dfa(automaton->automatons[i], categories)) {
            return FALSE;
        }
    }
    return TRUE;
}

int accept_string_nfa(NFA *automaton, const char *string) {
    for (int i = 0; i < automaton->size; i++) {
        if (accept_string_dfa(automaton->automatons[i], string)) {
            return TRUE;
        }
    }
    return FALSE;
}

void clear_nfa(NFA *nfa) {
    if (!nfa) { return; }
    for (int i = 0; i < nfa->size; i++) {
        delete_dfa(&nfa->automatons[i]);
    }
    free(nfa->automatons);
    nfa->automatons = NULL;
    nfa->size = 0;
}

void delete_nfa(NFA **nfa) {
    if (!*nfa) { return; }
    clear_nfa(*nfa);
    free(*nfa);
    *nfa = NULL;
} 

void print_nfa(const NFA *automaton) {
    if (!automaton) { return; }

    printf("{\n");
    printf("dfa size: %d\n", automaton->size);
    printf("dfa list: \n");

    printf("[\n");
    for (int i = 0; i < automaton->size; i++) {
        print_dfa(automaton->automatons[i]);
    }
    printf("]\n");
    printf("}\n");
}
