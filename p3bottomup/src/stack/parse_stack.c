/**
 * @title: Parse Stack Module for P3 Bottom-Up Parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_stack.h"

static const char *symbol_name_by_id(const LanguageSpec *spec, int symbol_id) {
    int i;

    if (!spec) {
        return "?";
    }

    for (i = 0; i < spec->symbol_count; i++) {
        if (spec->symbols[i].id == symbol_id) {
            return spec->symbols[i].name;
        }
    }

    return "?";
}

ParseStack *create_parse_stack(int initial_capacity) {
    ParseStack *stack;

    if (initial_capacity <= 0) {
        initial_capacity = 16;
    }

    stack = (ParseStack *)calloc(1, sizeof(ParseStack));
    if (!stack) {
        return NULL;
    }

    stack->items = (StackEntry *)calloc(initial_capacity, sizeof(StackEntry));
    if (!stack->items) {
        free(stack);
        return NULL;
    }

    stack->capacity = initial_capacity;
    stack->size = 0;
    return stack;
}

int push_stack(ParseStack *stack, const StackEntry *entry) {
    StackEntry *next_items;

    if (!stack || !entry) {
        return FALSE;
    }

    if (stack->size >= stack->capacity) {
        int next_capacity = stack->capacity * 2;
        next_items = (StackEntry *)realloc(stack->items, next_capacity * sizeof(StackEntry));
        if (!next_items) {
            return FALSE;
        }
        stack->items = next_items;
        stack->capacity = next_capacity;
    }

    stack->items[stack->size++] = *entry;
    return TRUE;
}

int pop_stack(ParseStack *stack, StackEntry *out) {
    if (!stack || stack->size <= 0) {
        return FALSE;
    }

    stack->size--;
    if (out) {
        *out = stack->items[stack->size];
    }

    return TRUE;
}

StackEntry *peek_stack(ParseStack *stack) {
    if (!stack || stack->size <= 0) {
        return NULL;
    }
    return &stack->items[stack->size - 1];
}

StackEntry *peek_stack_n(const ParseStack *stack, int n_from_top) {
    int idx;

    if (!stack || n_from_top < 0) {
        return NULL;
    }

    idx = stack->size - 1 - n_from_top;
    if (idx < 0 || idx >= stack->size) {
        return NULL;
    }

    return (StackEntry *)&stack->items[idx];
}

void free_parse_stack(ParseStack **stack) {
    if (!stack || !*stack) {
        return;
    }

    free((*stack)->items);
    (*stack)->items = NULL;
    (*stack)->size = 0;
    (*stack)->capacity = 0;

    free(*stack);
    *stack = NULL;
}

void format_stack(const ParseStack *stack, const LanguageSpec *spec, char *out, int out_len) {
    int i;
    int used = 0;

    if (!out || out_len <= 0) {
        return;
    }

    out[0] = '\0';

    if (!stack) {
        snprintf(out, out_len, "[]");
        return;
    }

    used += snprintf(out + used, out_len - used, "[");

    for (i = 0; i < stack->size; i++) {
        const char *symbol_name;
        if (i > 0) {
            used += snprintf(out + used, out_len - used, " ");
        }

        if (stack->items[i].symbol_id < 0) {
            symbol_name = "$ROOT";
        } else {
            symbol_name = symbol_name_by_id(spec, stack->items[i].symbol_id);
        }

        used += snprintf(
            out + used,
            out_len - used,
            "(%d,%s)",
            stack->items[i].state,
            symbol_name
        );

        if (used >= out_len - 1) {
            break;
        }
    }

    snprintf(out + used, out_len - used, "]");
}
