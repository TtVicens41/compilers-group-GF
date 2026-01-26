#include "symbol_table.h"
#include <string.h>

void symbol_table_init(SymbolTable *table) {
    if (!table) return;
    table->count = 0;
    memset(table->identifiers, 0, sizeof(table->identifiers));
    memset(table->values, 0, sizeof(table->values));
    memset(table->is_macro, 0, sizeof(table->is_macro));
    memset(table->parameters, 0, sizeof(table->parameters));
}

static int find_symbol_index(const SymbolTable *table, const char *identifier) {
    if (!table || !identifier) return -1;
    
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->identifiers[i], identifier) == 0) {
            return i;
        }
    }
    return -1;
}

bool symbol_table_add(SymbolTable *table, const char *identifier) {
    return symbol_table_add_with_value(table, identifier, "");
}

bool symbol_table_add_with_value(SymbolTable *table, const char *identifier, const char *value) {
    if (!table || !identifier) return false;
    
    if (table->count >= MAX_SYMBOLS) return false;
    
    // Check if already exists (update value if it does)
    int idx = find_symbol_index(table, identifier);
    if (idx >= 0) {
        if (value) {
            strncpy(table->values[idx], value, MAX_VALUE_LEN - 1);
            table->values[idx][MAX_VALUE_LEN - 1] = '\0';
        }
        table->is_macro[idx] = false;
        return true;
    }
    
    // Add new entry
    strncpy(table->identifiers[table->count], identifier, MAX_IDENTIFIER_LEN - 1);
    table->identifiers[table->count][MAX_IDENTIFIER_LEN - 1] = '\0';
    
    if (value) {
        strncpy(table->values[table->count], value, MAX_VALUE_LEN - 1);
        table->values[table->count][MAX_VALUE_LEN - 1] = '\0';
    } else {
        table->values[table->count][0] = '\0';
    }
    
    table->is_macro[table->count] = false;
    table->parameters[table->count][0] = '\0';
    table->count++;
    return true;
}

bool symbol_table_add_macro(SymbolTable *table, const char *identifier,
                            const char *parameters, const char *body) {
    if (!table || !identifier) return false;
    
    if (table->count >= MAX_SYMBOLS) return false;
    
    // Check if already exists (update if it does)
    int idx = find_symbol_index(table, identifier);
    if (idx >= 0) {
        if (body) {
            strncpy(table->values[idx], body, MAX_VALUE_LEN - 1);
            table->values[idx][MAX_VALUE_LEN - 1] = '\0';
        }
        if (parameters) {
            strncpy(table->parameters[idx], parameters, MAX_VALUE_LEN - 1);
            table->parameters[idx][MAX_VALUE_LEN - 1] = '\0';
        }
        table->is_macro[idx] = true;
        return true;
    }
    
    // Add new macro
    strncpy(table->identifiers[table->count], identifier, MAX_IDENTIFIER_LEN - 1);
    table->identifiers[table->count][MAX_IDENTIFIER_LEN - 1] = '\0';
    
    if (body) {
        strncpy(table->values[table->count], body, MAX_VALUE_LEN - 1);
        table->values[table->count][MAX_VALUE_LEN - 1] = '\0';
    } else {
        table->values[table->count][0] = '\0';
    }
    
    if (parameters) {
        strncpy(table->parameters[table->count], parameters, MAX_VALUE_LEN - 1);
        table->parameters[table->count][MAX_VALUE_LEN - 1] = '\0';
    } else {
        table->parameters[table->count][0] = '\0';
    }
    
    table->is_macro[table->count] = true;
    table->count++;
    return true;
}

bool symbol_table_contains(const SymbolTable *table, const char *identifier) {
    return find_symbol_index(table, identifier) >= 0;
}

const char *symbol_table_get_value(const SymbolTable *table, const char *identifier) {
    int idx = find_symbol_index(table, identifier);
    return (idx >= 0) ? table->values[idx] : NULL;
}

bool symbol_table_is_macro(const SymbolTable *table, const char *identifier) {
    int idx = find_symbol_index(table, identifier);
    return (idx >= 0) ? table->is_macro[idx] : false;
}

const char *symbol_table_get_parameters(const SymbolTable *table, const char *identifier) {
    int idx = find_symbol_index(table, identifier);
    return (idx >= 0 && table->is_macro[idx]) ? table->parameters[idx] : NULL;
}
