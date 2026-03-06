/**
 * @file action.h
 * @brief Module for Action data structure for Deterministic Finite-State 
 * Automaton
 * @author Marc Bosch Manzano
 * @since 2026/02/27
 */

#ifndef ACTION_H
#define ACTION_H

typedef enum {
    ACTION_SHIFT,
    ACTION_REDUCE,
    ACTION_ACCEPT,
    ACTION_ERROR
} ActionType;

typedef struct {
    ActionType type; /* The action type: SHIFT, REDUCE, ACCEPT, or ERROR. */
    int value;       /* Target state (SHIFT) or production id (REDUCE) */
} Action;

/** Inline string representation for an action. */
char *action_string(const Action *action);

/** Inline string representation for action row of an action table. */
char *action_row_string(const Action *actions, int columns, int level);

/**
 * String representation of an action table.
 * @param actions A matrix of actions where rows are states and columns are 
 * terminals.
 * @param rows Maximum rows to convert to string for action table.
 * @param rows Maximum columns to convert to string for action table.
 * @param level The indentation level of the action table when it is part 
 * of another data structure.
 * @return A dynamically allocated string for the action table.
 */
char *action_table_string(Action **actions, int rows, int columns, int level);

#endif /* ACTION_H */