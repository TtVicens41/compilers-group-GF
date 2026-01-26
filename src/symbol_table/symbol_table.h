#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

#define MAX_SYMBOLS 256
#define MAX_IDENTIFIER_LEN 64

/*
 * SymbolTable
 * -----------
 * Simple table for storing #define identifiers.
 * For now, we only store the identifier name (no value).
 */
typedef struct {
    char identifiers[MAX_SYMBOLS][MAX_IDENTIFIER_LEN];
    int count;
} SymbolTable;

/*
 * symbol_table_init
 * -----------------
 * Initialize an empty symbol table.
 */
void symbol_table_init(SymbolTable *table);

/*
 * symbol_table_add
 * ----------------
 * Add an identifier to the symbol table.
 * Returns true on success, false if table is full.
 */
bool symbol_table_add(SymbolTable *table, const char *identifier);

/*
 * symbol_table_contains
 * ---------------------
 * Check if an identifier exists in the symbol table.
 * Returns true if found, false otherwise.
 */
bool symbol_table_contains(const SymbolTable *table, const char *identifier);

#endif
