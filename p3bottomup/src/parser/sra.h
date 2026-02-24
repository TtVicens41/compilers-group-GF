/**
 * @file sra.h
 * @brief Shift / Reduce Automaton engine.
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

typedef struct {
    DFA      *dfa;
    Stack    *stack;
    Language *language;
} SRA;

/** Allocate and initialise the SRA (pushes initial state 0). */
SRA *sra_init(DFA *dfa, Stack *stack, Language *language);

/**
 * Run the full parse loop over a token list.
 * @param logger  Debug logger (may be NULL to suppress output).
 * @return 0 on accept, non-zero on error.
 */
int sra_parse(SRA *sra, TokenList *tokens, Logger *logger);

/** Execute a SHIFT operation (push token, advance input). */
void sra_shift(SRA *sra, int symbol, int target_state);

/** Execute a REDUCE operation (pop rhs, push lhs via GOTO). */
void sra_reduce(SRA *sra, const Production *prod);

/** Free the SRA struct (does NOT free the components it wraps). */
void sra_destroy(SRA *sra);

#endif /* SRA_H */
