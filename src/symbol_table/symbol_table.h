#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

#define MAX_SYMBOLS 256
#define MAX_IDENTIFIER_LEN 64
#define MAX_VALUE_LEN 256

/*
 * SymbolTable
 * -----------
 * Table for storing #define identifiers with their values.
 * Supports both simple constants and macros with parameters.
 */
typedef struct {
    char identifiers[MAX_SYMBOLS][MAX_IDENTIFIER_LEN];
    char values[MAX_SYMBOLS][MAX_VALUE_LEN];
    bool is_macro[MAX_SYMBOLS];  // True if this is a macro with parameters
    char parameters[MAX_SYMBOLS][MAX_VALUE_LEN];  // Parameter list for macros
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
 * Add an identifier to the symbol table (for ifdef).
 * Returns true on success, false if table is full.
 */
bool symbol_table_add(SymbolTable *table, const char *identifier);

/*
 * symbol_table_add_with_value
 * ---------------------------
 * Add an identifier with its value to the symbol table (for define).
 * Returns true on success, false if table is full.
 */
bool symbol_table_add_with_value(SymbolTable *table, const char *identifier, const char *value);

/*
 * symbol_table_add_macro
 * ----------------------
 * Add a macro with parameters to the symbol table.
 * Returns true on success, false if table is full.
 */
bool symbol_table_add_macro(SymbolTable *table, const char *identifier, 
                            const char *parameters, const char *body);

/*
 * symbol_table_contains
 * ---------------------
 * Check if an identifier exists in the symbol table.
 * Returns true if found, false otherwise.
 */
bool symbol_table_contains(const SymbolTable *table, const char *identifier);

/*
 * symbol_table_get_value
 * ----------------------
 * Get the value associated with an identifier.
 * Returns the value string, or NULL if not found.
 */
const char *symbol_table_get_value(const SymbolTable *table, const char *identifier);

/*
 * symbol_table_is_macro
 * ---------------------
 * Check if an identifier is a macro.
 * Returns true if it's a macro, false otherwise.
 */
bool symbol_table_is_macro(const SymbolTable *table, const char *identifier);

/*
 * symbol_table_get_parameters
 * ---------------------------
 * Get the parameter list for a macro.
 * Returns the parameter string, or NULL if not found or not a macro.
 */
const char *symbol_table_get_parameters(const SymbolTable *table, const char *identifier);

#endif
