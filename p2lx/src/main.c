/**
 * @title: Main Program for Lexer/Scanner
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
 */

#include <stdio.h>

#include "automata/automata.h"
#include "language_defs.h"
#include "token.h"

static int COUNT = 0;

int main(void) {
    NFA *nfa = read_union_nfa(AUTOMATA_FILE);
    
    DFA *dfa = nfa->automatas[9];
    print_dfa(dfa);
    printf("%d\n", is_accepted_dfa(dfa, ";"));
    printf("%d\n", is_accepted_nfa(nfa, "if"));

    print_token((void *)get_token_nfa(nfa, "\"asdasdas\""));
}