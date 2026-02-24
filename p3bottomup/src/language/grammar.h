/**
 * @file grammar.h
 * @brief Query helpers for Language data.
 *
 * Provides read-only accessor and formatting utilities that operate on a
 * loaded Language instance. No module outside the language layer should
 * access Symbol / Production fields directly; use these functions instead.
 */

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "language.h"

/** Return the human-readable name of a symbol, or "?" if not found. */
const char *grammar_symbol_name(const Language *lang, int symbol_id);

/** Return the type (terminal / nonterminal) of a symbol. */
SymbolType grammar_symbol_type(const Language *lang, int symbol_id);

/** Return a pointer to the Production with the given id, or NULL. */
const Production *grammar_get_production(const Language *lang, int prod_id);

/** Find a symbol id by name. Returns -1 if not found. */
int grammar_find_symbol_by_name(const Language *lang, const char *name);

/**
 * Format a production as a human-readable string.
 * Caller must free() the returned buffer.
 *   e.g. "e -> e + t"
 */
char *grammar_format_production(const Language *lang, int prod_id);

/** Count the number of terminal symbols in the language. */
int grammar_terminal_count(const Language *lang);

/** Count the number of nonterminal symbols in the language. */
int grammar_nonterminal_count(const Language *lang);

#endif /* GRAMMAR_H */
