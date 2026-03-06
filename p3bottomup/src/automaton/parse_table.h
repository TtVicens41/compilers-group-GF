/**
 * @file parse_table.h
 * @brief ACTION / GOTO parse table loaded from a data file.
 */

#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

#include "action.h"

/**
 * @brief ACTION / GOTO parse table data structure.
 */
typedef struct {
    Action **action_table;  /* A matrix of actions: [state][terminal_id] */
    int **goto_table;       /* A goto matrix: [state][nonterminal_offset] */
    int state_count;        /* Number of states. */
    int terminal_count;     /* Number of terminal symbols. */
    int nonterminal_count;  /* Number of nonterminal symbols. */
} ParseTable;

/**
 * Load a parse table from a text file.
 * @param path  Path to the parse-table definition file.
 * @return Heap-allocated ParseTable, or NULL on failure.
 */
ParseTable *parse_table_load_from_file(const char *path);

/** Look up an ACTION entry. Returns ACTION_ERROR for out-of-range. */
Action parse_table_get_action(const ParseTable *table, int state,
                              int terminal_id);

/**
 * Look up a GOTO entry.
 * @param nonterminal_id  The Language-level symbol id (not the offset).
 * @return Target state, or -1 on error / empty cell.
 */
int parse_table_get_goto(const ParseTable *table, int state,
                         int nonterminal_id);

/** Free all memory owned by a ParseTable. */
void parse_table_destroy(ParseTable *table);

/** Dynamically allocated string representation of parse table. */
char *parse_table_string(const ParseTable *table, int level);

#endif /* PARSE_TABLE_H */
