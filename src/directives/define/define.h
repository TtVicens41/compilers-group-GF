/**
 * Made by Joan Vicente Martín (19/01/2026)
 * 
 * Define directive module for the C preprocessor.
 * Handles #define directives: stores macro definitions and performs
 * text substitution on subsequent lines.
 */

#ifndef DEFINE_H
#define DEFINE_H

#include <stddef.h>
#include <stdbool.h>

/* Initial capacity for the define table */
#define DEFINE_TABLE_INITIAL_CAPACITY 16

/* Maximum length for a macro key (identifier) */
#define DEFINE_KEY_MAX_LENGTH 256

/* Maximum length for a macro value */
#define DEFINE_VALUE_MAX_LENGTH 4096

/**
 * DefineEntry
 * -----------
 * Represents a single #define macro definition.
 * key:   The identifier being defined (e.g., "MAX")
 * value: The replacement text (e.g., "100")
 */
typedef struct {
    char *key;
    char *value;
} DefineEntry;

/**
 * DefineTable
 * -----------
 * Dynamic array that stores all active macro definitions.
 * entries:  Array of DefineEntry pointers
 * count:    Number of entries currently stored
 * capacity: Total allocated slots in the array
 */
typedef struct {
    DefineEntry *entries;
    size_t count;
    size_t capacity;
} DefineTable;

/**
 * Initializes a DefineTable with default capacity.
 * Must be called before using the table.
 * Returns true on success, false on allocation failure.
 */
bool define_table_init(DefineTable *table);

/**
 * Frees all memory associated with the DefineTable.
 * Safe to call on an already-freed or uninitialized table.
 */
void define_table_free(DefineTable *table);

/**
 * Adds or updates a macro definition in the table.
 * If the key already exists, its value is replaced.
 * Returns true on success, false on allocation failure.
 */
bool define_table_set(DefineTable *table, const char *key, const char *value);

/**
 * Looks up a macro definition by key.
 * Returns the value string if found, NULL otherwise.
 * The returned pointer is owned by the table; do not free it.
 */
const char *define_table_get(const DefineTable *table, const char *key);

/**
 * Parses a #define directive line and adds the definition to the table.
 * Expected format: #define <key> <value>
 * - key: identifier with no spaces
 * - value: everything after the first space following the key
 * 
 * line_number: used for error reporting
 * Returns true if parsing succeeded, false if malformed.
 */
bool handle_define(DefineTable *table, const char *line, int line_number);

/**
 * Applies all macro substitutions to an input line.
 * Replaces whole-word occurrences of defined macros.
 * 
 * Returns a newly allocated string with substitutions applied.
 * Caller is responsible for freeing the returned string.
 * Returns NULL on allocation failure.
 */
char *apply_substitutions(const DefineTable *table, const char *line);

#endif /* DEFINE_H */
