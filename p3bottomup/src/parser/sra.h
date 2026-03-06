/**
 * @file sra.h
 * @brief Shift-Reduce Automaton (SRA) engine.
 *
 * Orchestrates the DFA and the Stack according to the Language grammar.
 * Contains no grammar-specific logic; behaviour is entirely table-driven.
 */

#ifndef SRA_H
#define SRA_H

#include "../automaton/dfa.h"
#include "../stack/stack.h"
#include "../language/language.h"
#include "../tokens/token_list.h"
#include "../utils/logger.h"

/**
 * @brief Shift-Reduce Automaton (SRA) data structure.
 */
typedef struct {
    DFA      *dfa;       /* A parser Deterministic Finite-State Automaton */
    Stack    *stack;     /* A generic stack for parser */
    Language *language;  /* A language for parser */
} SRA;

/** 
 * Allocate and initialise the SRA (pushes initial state 0). 
 * @param dfa An initialized deterministic finite-state automaton.
 * @param stack An initialized stack.
 * @param language An initialized language.
 * @return A pointer to Shift-Reduce Automaton (NULL when failure to 
 * allocation).
 */
SRA *sra_init(DFA *dfa, Stack *stack, Language *language);

/**
 * Run the full parse loop over a token list.
 * @param sra An initialized read-write Shift-Reduce Automaton.
 * @param tokens An initialized token list.
 * @param logger  Debug logger (may be NULL to suppress output).
 * @return EXIT_SUCCESS (0) on accept, EXIT_FAILURE (1) on error.
 */
int sra_parse(SRA *sra, TokenList *tokens, Logger *logger);

/** 
 * Execute a SHIFT operation: push token, advance input. 
 * @param sra An initialized read-write Shift-Reduce Automaton.
 * @param symbol A symbol to be pushed.
 * @param target_state A target state to be pushed.
 */
void sra_shift(SRA *sra, int symbol, int target_state);

/** 
 * Execute a REDUCE operation: pop rhs, push lhs via GOTO.
 * @param sra An initialized read-write Shift-Reduce Automaton.
 * @param prod A read-only production rule.
 */
void sra_reduce(SRA *sra, const Production *prod);

/** 
 * Free the SRA struct (does NOT free the components it wraps).
 * @param sra An allocated Shift-Reduce Automaton to be freed.
 */
void sra_destroy(SRA *sra);

/** Dynamically allocated string representation of SRA. */
char *sra_string(const SRA *sra, int level);

#endif /* SRA_H */
