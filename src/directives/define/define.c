/**
 * Made by Joan Vicente Martín (19/01/2026)
 * 
 * Implementation of the #define directive handler.
 * Manages a table of macro definitions and performs whole-word
 * substitution on lines processed by the preprocessor.
 */

#include "define.h"
#include "../../utils/string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* =========================================================================
 * HELPER FUNCTIONS (module-specific)
 * ========================================================================= */

/**
 * Finds the index of a key in the table, or -1 if not found.
 */
static int find_entry_index(const DefineTable *table, const char *key) {
    if (table == NULL || key == NULL) return -1;
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].key, key) == 0) {
            return (int)i;
        }
    }
    return -1;
}

/**
 * Grows the table capacity by doubling it.
 * Returns true on success, false on allocation failure.
 */
static bool grow_table(DefineTable *table) {
    size_t new_capacity = table->capacity * 2;
    DefineEntry *new_entries = realloc(table->entries, 
                                        new_capacity * sizeof(DefineEntry));
    if (new_entries == NULL) {
        return false;
    }
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

/* =========================================================================
 * TABLE MANAGEMENT
 * ========================================================================= */

/**
 * Initializes a DefineTable with default capacity.
 */
bool define_table_init(DefineTable *table) {
    if (table == NULL) return false;
    
    table->entries = malloc(DEFINE_TABLE_INITIAL_CAPACITY * sizeof(DefineEntry));
    if (table->entries == NULL) {
        return false;
    }
    table->count = 0;
    table->capacity = DEFINE_TABLE_INITIAL_CAPACITY;
    return true;
}

/**
 * Frees all memory associated with the DefineTable.
 */
void define_table_free(DefineTable *table) {
    if (table == NULL) return;
    
    /* Free each entry's key and value */
    for (size_t i = 0; i < table->count; i++) {
        free(table->entries[i].key);
        free(table->entries[i].value);
    }
    
    /* Free the entries array */
    free(table->entries);
    
    /* Reset the structure */
    table->entries = NULL;
    table->count = 0;
    table->capacity = 0;
}

/**
 * Adds or updates a macro definition in the table.
 */
bool define_table_set(DefineTable *table, const char *key, const char *value) {
    if (table == NULL || key == NULL) return false;
    
    /* Check if key already exists - if so, update value */
    int existing_idx = find_entry_index(table, key);
    if (existing_idx >= 0) {
        /* Free old value and replace */
        free(table->entries[existing_idx].value);
        table->entries[existing_idx].value = str_duplicate(value ? value : "");
        return table->entries[existing_idx].value != NULL;
    }
    
    /* Need to add a new entry - check capacity */
    if (table->count >= table->capacity) {
        if (!grow_table(table)) {
            return false;
        }
    }
    
    /* Add new entry */
    char *key_copy = str_duplicate(key);
    char *value_copy = str_duplicate(value ? value : "");
    
    if (key_copy == NULL || value_copy == NULL) {
        free(key_copy);
        free(value_copy);
        return false;
    }
    
    table->entries[table->count].key = key_copy;
    table->entries[table->count].value = value_copy;
    table->count++;
    
    return true;
}

/**
 * Looks up a macro definition by key.
 */
const char *define_table_get(const DefineTable *table, const char *key) {
    if (table == NULL || key == NULL) return NULL;
    
    int idx = find_entry_index(table, key);
    if (idx >= 0) {
        return table->entries[idx].value;
    }
    return NULL;
}

/* =========================================================================
 * DIRECTIVE PARSING
 * ========================================================================= */

/**
 * Parses a #define directive line and adds the definition to the table.
 * Expected format: #define <key> <value>
 * 
 */
bool handle_define(DefineTable *table, const char *line, int line_number) {
    if (table == NULL || line == NULL) return false;
    
    const char *p = line;
    
    /* Skip leading whitespace */
    while (*p && isspace((unsigned char)*p)) p++;
    
    /* Skip '#' */
    if (*p != '#') {
        fprintf(stderr, "Error at line %d: Not a directive\n", line_number);
        return false;
    }
    p++;
    
    /* Skip whitespace after '#' */
    while (*p && isspace((unsigned char)*p)) p++;
    
    /* Skip "define" keyword */
    if (strncmp(p, "define", 6) != 0) {
        fprintf(stderr, "Error at line %d: Expected 'define' keyword\n", line_number);
        return false;
    }
    p += 6;
    
    /* Must have whitespace after "define" */
    if (!isspace((unsigned char)*p)) {
        fprintf(stderr, "Error at line %d: Missing space after 'define'\n", line_number);
        return false;
    }
    
    /* Skip whitespace to reach the key */
    while (*p && isspace((unsigned char)*p)) p++;
    
    /* Extract the key (first token, no spaces allowed in key) */
    char key[DEFINE_KEY_MAX_LENGTH];
    size_t key_len = 0;
    
    while (*p && !isspace((unsigned char)*p) && key_len < DEFINE_KEY_MAX_LENGTH - 1) {
        key[key_len++] = *p++;
    }
    key[key_len] = '\0';
    
    /* Key must not be empty */
    if (key_len == 0) {
        fprintf(stderr, "Error at line %d: #define missing identifier\n", line_number);
        return false;
    }
    
    /* Skip whitespace between key and value */
    while (*p && isspace((unsigned char)*p) && *p != '\n') p++;
    
    /* Extract the value (everything until end of line, trimmed) */
    char value[DEFINE_VALUE_MAX_LENGTH];
    size_t value_len = 0;
    
    /* Copy value, stopping at newline */
    while (*p && *p != '\n' && *p != '\r' && value_len < DEFINE_VALUE_MAX_LENGTH - 1) {
        value[value_len++] = *p++;
    }
    value[value_len] = '\0';
    
    /* Trim trailing whitespace from value */
    while (value_len > 0 && isspace((unsigned char)value[value_len - 1])) {
        value[--value_len] = '\0';
    }
    
    /* Store the definition */
    if (!define_table_set(table, key, value)) {
        fprintf(stderr, "Error at line %d: Failed to store definition for '%s'\n", 
                line_number, key);
        return false;
    }
    
    return true;
}

/* =========================================================================
 * SUBSTITUTION
 * ========================================================================= */

/**
 * Applies all macro substitutions to an input line.
 * Uses whole-word matching: a key is only replaced if it is not
 * surrounded by identifier characters (A-Za-z0-9_).
 * 
 * Returns a newly allocated string. Caller must free it.
 */
char *apply_substitutions(const DefineTable *table, const char *line) {
    if (line == NULL) return NULL;
    if (table == NULL || table->count == 0) {
        /* No substitutions to apply, return a copy */
        return str_duplicate(line);
    }
    
    /* Start with a copy of the line */
    size_t result_capacity = strlen(line) * 2 + 1;
    char *result = malloc(result_capacity);
    if (result == NULL) return NULL;
    
    strcpy(result, line);
    
    /* Apply substitutions for each defined macro */
    for (size_t i = 0; i < table->count; i++) {
        const char *key = table->entries[i].key;
        const char *value = table->entries[i].value;
        size_t key_len = strlen(key);
        size_t value_len = strlen(value);
        
        /* Scan through result looking for whole-word matches */
        char *pos = result;
        while ((pos = strstr(pos, key)) != NULL) {
            /* Check if this is a whole-word match */
            bool at_start = (pos == result) || !is_identifier_char(*(pos - 1));
            bool at_end = !is_identifier_char(*(pos + key_len));
            
            if (at_start && at_end) {
                /* Whole-word match: perform substitution */
                size_t prefix_len = pos - result;
                size_t suffix_len = strlen(pos + key_len);
                size_t new_len = prefix_len + value_len + suffix_len;
                
                /* Ensure enough capacity */
                if (new_len + 1 > result_capacity) {
                    result_capacity = new_len * 2 + 1;
                    char *new_result = realloc(result, result_capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    /* Update pos after realloc */
                    pos = new_result + prefix_len;
                    result = new_result;
                }
                
                /* Shift suffix to make room for value */
                memmove(pos + value_len, pos + key_len, suffix_len + 1);
                
                /* Copy value in place */
                memcpy(pos, value, value_len);
                
                /* Continue searching after the substitution */
                pos += value_len;
            } else {
                /* Not a whole-word match, skip this occurrence */
                pos += key_len;
            }
        }
    }
    
    return result;
}
