#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/file_utils.h"
#include "../utils/string_list.h"
#include "../utils/string_utils.h"
#include "automata.h"

static DFA *empty_dfa(void) {
    return (DFA *)calloc(1, sizeof(DFA));
}

static NFA *empty_nfa(void) {
    return (NFA *)calloc(1, sizeof(NFA));
}

static int *to_integer_array_exact(StringList *string_list, int expected_cols,
                                   int delete_list) {
    int *row;
    int i;

    row = (int *)calloc(expected_cols, sizeof(int));
    if (!row) {
        return NULL;
    }

    if (string_list) {
        int cols = string_list->size < expected_cols ? string_list->size : expected_cols;
        for (i = 0; i < cols; i++) {
            row[i] = atoi(string_list->buffer[i]);
        }
        if (delete_list) {
            delete_string_list(&string_list);
        }
    }

    return row;
}

DFA *read_dfa(char *automaton_string) {
    DFA *dfa;
    StringList *string_splitted;
    StringList *accepting_states;
    char **buffer;
    int lines;
    int j;

    string_splitted = string_split(automaton_string, '\n');
    if (!string_splitted || string_splitted->size < 5) {
        return NULL;
    }

    buffer = string_splitted->buffer;
    lines = string_splitted->size;

    dfa = empty_dfa();
    if (!dfa) {
        delete_string_list(&string_splitted);
        return NULL;
    }

    dfa->category = token_category_from_text(buffer[0]);
    dfa->alphabet = get_copy(buffer[1]);
    dfa->alphabet_size = (int)strlen(buffer[1]);
    dfa->char_map = init_char_map(buffer[1]);

    dfa->states_size = atoi(buffer[2]);
    dfa->initial_state = atoi(buffer[3]);

    accepting_states = string_split(buffer[4], ' ');
    dfa->accepting_states_size = accepting_states ? accepting_states->size : 0;
    dfa->accepting_states = to_integer_array(accepting_states, 1);

    dfa->transitions = (int **)calloc(dfa->states_size, sizeof(int *));
    if (!dfa->transitions) {
        free_dfa(dfa);
        delete_string_list(&string_splitted);
        return NULL;
    }

    for (j = 5; j < lines && (j - 5) < dfa->states_size; j++) {
        StringList *transition = string_split(buffer[j], ' ');
        dfa->transitions[j - 5] = to_integer_array_exact(transition, dfa->alphabet_size, 1);
    }

    for (j = lines - 5; j < dfa->states_size; j++) {
        if (j >= 0 && !dfa->transitions[j]) {
            dfa->transitions[j] = (int *)calloc(dfa->alphabet_size, sizeof(int));
        }
    }

    delete_string_list(&string_splitted);
    return dfa;
}

static DFA **init_dfa_list(StringList *automata_strings) {
    int i;
    DFA **automata_list;

    automata_list = (DFA **)calloc(automata_strings->size, sizeof(DFA *));
    if (!automata_list) {
        return NULL;
    }

    for (i = 0; i < automata_strings->size; i++) {
        automata_list[i] = read_dfa(automata_strings->buffer[i]);
    }

    return automata_list;
}

NFA *read_union_nfa(const char *file) {
    char *buffer;
    StringList *automata_strings;
    NFA *nfa;

    buffer = read_file(file);
    if (!buffer) {
        return NULL;
    }

    automata_strings = apply_string_list(string_split(buffer, '#'), trim_string, 1);
    free(buffer);

    if (!automata_strings) {
        return NULL;
    }

    nfa = empty_nfa();
    if (!nfa) {
        delete_string_list(&automata_strings);
        return NULL;
    }

    nfa->size = automata_strings->size;
    nfa->automatas = init_dfa_list(automata_strings);

    delete_string_list(&automata_strings);
    return nfa;
}

int is_accepted_dfa(const DFA *automaton, const char *string) {
    int state;
    int i;
    int len;

    if (!automaton || !string) {
        return 0;
    }

    state = automaton->initial_state;
    len = (int)strlen(string);

    for (i = 0; i < len; i++) {
        int symbol = automaton->char_map[(unsigned char)string[i]];

        if (symbol == KEY_ERROR) {
            state = EMPTY_STATE;
            break;
        }

        if (state < 0 || state >= automaton->states_size) {
            state = EMPTY_STATE;
            break;
        }

        state = automaton->transitions[state][symbol];
    }

    for (i = 0; i < automaton->accepting_states_size; i++) {
        if (state == automaton->accepting_states[i]) {
            return 1;
        }
    }

    return 0;
}

TokenCategory classify_lexeme_nfa(const NFA *automaton, const char *string) {
    int i;

    if (!automaton || !string) {
        return CAT_NONRECOGNIZED;
    }

    for (i = 0; i < automaton->size; i++) {
        if (automaton->automatas[i] &&
            is_accepted_dfa(automaton->automatas[i], string)) {
            return automaton->automatas[i]->category;
        }
    }

    return CAT_NONRECOGNIZED;
}

void free_dfa(DFA *automaton) {
    int i;
    if (!automaton) {
        return;
    }

    free(automaton->alphabet);
    free(automaton->accepting_states);
    free(automaton->char_map);

    if (automaton->transitions) {
        for (i = 0; i < automaton->states_size; i++) {
            free(automaton->transitions[i]);
        }
    }
    free(automaton->transitions);
    free(automaton);
}

void free_nfa(NFA *automaton) {
    int i;
    if (!automaton) {
        return;
    }

    if (automaton->automatas) {
        for (i = 0; i < automaton->size; i++) {
            free_dfa(automaton->automatas[i]);
        }
    }

    free(automaton->automatas);
    free(automaton);
}

void print_dfa(const DFA *automaton) {
    int i;
    int j;

    if (!automaton) {
        return;
    }

    printf("{\n");
    printf("category: %s\n", token_category_text(automaton->category));
    printf("alphabet: %s\n", automaton->alphabet);
    printf("states_size: %d\n", automaton->states_size);
    printf("initial_state: %d\n", automaton->initial_state);
    printf("accepting_states:");
    for (i = 0; i < automaton->accepting_states_size; i++) {
        printf(" %d", automaton->accepting_states[i]);
    }
    printf("\ntransitions:\n");
    for (i = 0; i < automaton->states_size; i++) {
        for (j = 0; j < automaton->alphabet_size; j++) {
            printf("%d ", automaton->transitions[i][j]);
        }
        printf("\n");
    }
    printf("}\n");
}

void print_nfa(const NFA *automaton) {
    int i;
    if (!automaton) {
        return;
    }

    for (i = 0; i < automaton->size; i++) {
        print_dfa(automaton->automatas[i]);
    }
}
