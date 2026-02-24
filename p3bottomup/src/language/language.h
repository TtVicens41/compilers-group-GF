/**
 * @file language.h
 * @brief Language definition: symbols, productions, and the language container.
 *
 * Provides the core data structures that describe any context-free grammar
 * in a grammar-independent way. The Language struct is loaded entirely from
 * data files so the engine never hard-codes grammar rules.
 */

#ifndef LANGUAGE_H
#define LANGUAGE_H

/* ── Symbol ─────────────────────────────────────────────────────────── */

typedef enum {
    SYMBOL_TERMINAL,
    SYMBOL_NONTERMINAL
} SymbolType;

typedef struct {
    char name[32];
    SymbolType type;
    int id;
} Symbol;

/* ── Production ─────────────────────────────────────────────────────── */

typedef struct {
    int id;
    int lhs;          /* symbol id of the left-hand side */
    int rhs_len;
    int rhs[10];      /* symbol ids on the right-hand side */
} Production;

/* ── Language ───────────────────────────────────────────────────────── */

typedef struct {
    Symbol *symbols;
    int symbol_count;

    Production *productions;
    int production_count;

    int start_symbol;
} Language;

/**
 * Load a Language from a text file.
 * @param path  Path to a language definition file.
 * @return Heap-allocated Language, or NULL on failure.
 */
Language *language_load_from_file(const char *path);

/**
 * Free all memory owned by a Language.
 */
void language_destroy(Language *lang);

#endif /* LANGUAGE_H */
