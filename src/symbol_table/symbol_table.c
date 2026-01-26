#include "symbol_table.h"
#include <string.h>

void symbol_table_init(SymbolTable *table) {
    if (!table) return;
    table->count = 0;
    memset(table->identifiers, 0, sizeof(table->identifiers));
}

bool symbol_table_add(SymbolTable *table, const char *identifier) {
    if (!table || !identifier) return false;
    
    // Check if table is full
    if (table->count >= MAX_SYMBOLS) {
        return false;
    }
    
    // Check if already exists (avoid duplicates)
    if (symbol_table_contains(table, identifier)) {
        return true; // Already exists, consider it success
    }
    
    // Add to table
    strncpy(table->identifiers[table->count], identifier, MAX_IDENTIFIER_LEN - 1);
    table->identifiers[table->count][MAX_IDENTIFIER_LEN - 1] = '\0';
    table->count++;
    
    return true;
}

bool symbol_table_contains(const SymbolTable *table, const char *identifier) {
    if (!table || !identifier) return false;
    
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->identifiers[i], identifier) == 0) {
            return true;
        }
    }
    
    return false;
}
