#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "action.h"
#include "../utils/string_utils.h"

char *action_string(const Action *action) 
{
    if (!action) {
        return NULL;
    }
    char *s = NULL;
    jsonify_wrap(&s, 0, 0, "{");
    jsonify(&s, 0, 1, 0, "type",  int_to_str(action->type));
    jsonify(&s, 0, 0, 0, "value", int_to_str(action->value));
    jsonify_wrap(&s, 0, 0, "}");
    return s;
}

char *action_row_string(const Action *actions, int columns, int level) 
{
    if (!actions) {
        return NULL;
    }
    char *s = NULL;
    jsonify_wrap(&s, level, 0, "[");
    for (int i = 0; i < columns; i++) {
        char *action = action_string(&actions[i]);
        jsonify_value(&s, 0, i < columns -1, 0, action);
    }
    jsonify_wrap(&s, 0, 0, "]");
    return s;
}

char *action_table_string(Action **actions, int rows, int columns, int level) 
{
    if (!actions) {
        return NULL;
    }
    char *s = get_copy("\n");
    int n = level + 1;
    jsonify_wrap(&s, level, 1, "[");
    for (int i = 0; i < rows; i++) {
        jsonify_value(
            &s, 
            0, 
            i < rows - 1,
            1, 
            action_row_string(actions[i], columns, n)
        );
    }
    jsonify_wrap(&s, level, 0, "]");
    return s;
}