/**
 * @file stack.c
 * @brief Dynamic stack for the shift/reduce automaton.
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "../utils/string_utils.h"

#define DEFAULT_CAPACITY 128
#define UNITIALIZED_TOP  (-1)

/* ── internal resize ───────────────────────────────────────────────── */

static void stack_grow(Stack *stack)
{
    int new_cap = stack->capacity * 2;
    StackItem *tmp = realloc(stack->items,
                             sizeof(StackItem) * (size_t)new_cap);
    if (!tmp) {
        fprintf(stderr, "stack: out of memory\n");
        exit(EXIT_FAILURE);
    }
    stack->items    = tmp;
    stack->capacity = new_cap;
}

/* ── public API ─────────────────────────────────────────────────────── */

Stack *stack_init(void)
{
    Stack *stack = malloc(sizeof(Stack));
    if (!stack) { 
        return NULL; 
    }

    size_t stack_size = sizeof(StackItem) * (size_t)DEFAULT_CAPACITY;
    stack->items      = malloc(stack_size);
    stack->top        = UNITIALIZED_TOP;
    stack->capacity   = DEFAULT_CAPACITY;

    if (!stack->items) { 
        free(stack); 
        return NULL; 
    }

    return stack;
}

void stack_push(Stack *stack, int state, int symbol)
{
    if (stack->top + 1 >= stack->capacity)
        stack_grow(stack);

    stack->top++;
    stack->items[stack->top].state  = state;
    stack->items[stack->top].symbol = symbol;
}

StackItem stack_pop(Stack *stack)
{
    if (stack->top < 0) {
        fprintf(stderr, "stack: pop on empty stack\n");
        exit(EXIT_FAILURE);
    }
    return stack->items[stack->top--];
}

StackItem stack_peek(const Stack *stack)
{
    if (stack->top < 0) {
        fprintf(stderr, "stack: peek on empty stack\n");
        exit(EXIT_FAILURE);
    }
    return stack->items[stack->top];
}

int stack_size(const Stack *stack)
{
    return stack->top + 1;
}

StackItem stack_get(const Stack *stack, int index)
{
    if (index < 0 || index > stack->top) {
        fprintf(stderr, "stack: index %d out of range [0..%d]\n",
                index, stack->top);
        exit(EXIT_FAILURE);
    }
    return stack->items[index];
}

void stack_print(const Stack *stack)
{
    for (int i = 0; i <= stack->top; i++) {
        printf("[%d,%d]", stack->items[i].state, stack->items[i].symbol);
    }
    printf("\n");
}

void stack_destroy(Stack *stack)
{
    if (!stack) return;
    free(stack->items);
    free(stack);
}

char *item_string(const StackItem *item) 
{
    if (!item) {
        return NULL;
    }
    char *s = NULL;
    jsonify_wrap(&s, 0, 0, "{");
    jsonify(&s, 0, 1, 0, "state",  int_to_str(item->state));
    jsonify(&s, 0, 0, 0, "symbol", int_to_str(item->symbol));
    jsonify_wrap(&s, 0, 0, "}");
    return s;
}

char *items_string(const StackItem *items, int count, int level) 
{
    if (!items) {
        return NULL;
    }
    char *s = get_copy(count > 0 ? "\n" : NULL);
    int n = level + 1;
    int corrected_level = (count > 0) * level;
    jsonify_wrap(&s, corrected_level, count > 0, "[");
    for (int i = 0; i < count; i++) {
        jsonify_value(&s, n, i < count - 1, 1, item_string(&items[i]));
    }
    jsonify_wrap(&s, corrected_level, 0, "]");
    return s;
}

char *stack_string(const Stack *stack, int level) 
{
    if (!stack) {
        return NULL;
    }
    char *s = get_copy("\n");
    int n = level + 1;
    jsonify_wrap(&s, level, 1, "{");
    jsonify(&s, n, 1, 1, "items",    items_string(stack->items, stack->top + 1, n));
    jsonify(&s, n, 1, 1, "capacity", int_to_str(stack->capacity));
    jsonify(&s, n, 0, 1, "top",      int_to_str(stack->top));
    jsonify_wrap(&s, level, 0, "}");
    return s;
}