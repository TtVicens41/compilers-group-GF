/**
 * @title: Symbolic Table.
 * @brief: Declares the data structe of the symbol table of the C preprocessor
 * directives and all the functionalities regarding this data structure.
 * @authors: Davi Panna-Mattos Dias de Paiva, Marc Bosch Manzano.
 * @creation: before 2026/01/27.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

#define MAX_SYMBOLS 256
#define MAX_IDENTIFIER_LEN 64
#define MAX_VALUE_LEN 256

/**
 * SymbolTable
 * -----------
 * Table for storing `#define` identifiers with their values.
 * Supports both simple constants and macros with parameters.
 */
typedef struct SymbolTable {
    char identifiers[MAX_SYMBOLS][MAX_IDENTIFIER_LEN];
    char values[MAX_SYMBOLS][MAX_VALUE_LEN];
    bool is_macro[MAX_SYMBOLS];  // True if this is a macro with parameters
    char parameters[MAX_SYMBOLS][MAX_VALUE_LEN];  // Parameter list for macros
    int count;
} SymbolTable;

/**
 * Initialize an empty symbol table.
 * @param table A symbol table pointer.
 */
void symbol_table_init(SymbolTable *table);

/** 
 * Add an identifier to the symbol table for `#ifdef` directive.
 * @param table A symbol table pointer.
 * @param identifier A string of an identifier.
 * @return True on success, false if table is full.
 */
bool symbol_table_add(SymbolTable *table, const char *identifier);

/**
 * Add an identifier with its value to the symbol table for `#define` directive.
 * @param table A symbol table pointer.
 * @param identifier A string of an identifier.
 * @param value A string of the value that has to replece the identifier.
 * @return True on success, false if table is full.
 */
bool symbol_table_add_with_value(
    SymbolTable *table, 
    const char *identifier, 
    const char *value
);

/**
 * Add a macro-instruction with parameters to the symbol table for `#define` 
 * directive.
 * @param table A symbol table pointer.
 * @param identifier A string of an identifier.
 * @param parameters A string of the parameters of the macro.
 * @param body A string of the content inside the macro.
 * @return True on success, false if table is full.
 */
bool symbol_table_add_macro(
    SymbolTable *table, 
    const char *identifier, 
    const char *parameters, 
    const char *body
);

/**
 * Check if an identifier exists in the symbol table.
 * @param table A symbol table pointer.
 * @param identifier A string of an identifier.
 * @return True if found, false otherwise.
 */
bool symbol_table_contains(
    const SymbolTable *table, 
    const char *identifier
);

/**
 * Get the value associated with an identifier.
 * @param table A symbol table pointer.
 * @param identifier A string of an identifier.
 * @return The value string, or NULL if not found.
 */
const char *symbol_table_get_value(
    const SymbolTable *table, 
    const char *identifier
);

/**
 * Check if an identifier is a macro.
 * @param table A symbol table pointer.
 * @param identifier A string of an identifier.
 * @return True if it's a macro, false otherwise.
 */
bool symbol_table_is_macro(
    const SymbolTable *table, 
    const char *identifier
);

/**
 * Get the parameter list for a macro.
 * @param table A symbol table pointer.
 * @param identifier A string of an identifier.
 * @return The parameter string, or NULL if not found or not a macro.
 */
const char *symbol_table_get_parameters(
    const SymbolTable *table, 
    const char *identifier
);

#endif
