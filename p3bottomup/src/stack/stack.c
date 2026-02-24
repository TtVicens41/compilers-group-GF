/**
 * @file stack.c
 * @brief Dynamic stack for the shift/reduce automaton.
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

#define DEFAULT_CAPACITY 64

Stack *stack_init(int capacity)
{
    if (capacity <= 0) capacity = DEFAULT_CAPACITY;

    Stack *s = malloc(sizeof(Stack));
    if (!s) return NULL;

    s->items    = malloc(sizeof(StackItem) * (size_t)capacity);
    s->top      = -1;
    s->capacity = capacity;

    if (!s->items) { free(s); return NULL; }
    return s;
}

/* ── internal resize ───────────────────────────────────────────────── */

static void stack_grow(Stack *stack)
{
    int new_cap = stack->capacity * 2;
    StackItem *tmp = realloc(stack->items,
                             sizeof(StackItem) * (size_t)new_cap);
    if (!tmp) {
        fprintf(stderr, "stack: out of memory\n");
        exit(1);
    }
    stack->items    = tmp;
    stack->capacity = new_cap;
}

/* ── public API ─────────────────────────────────────────────────────── */

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
        exit(1);
    }
    return stack->items[stack->top--];
}

StackItem stack_peek(const Stack *stack)
{
    if (stack->top < 0) {
        fprintf(stderr, "stack: peek on empty stack\n");
        exit(1);
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
        exit(1);
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
