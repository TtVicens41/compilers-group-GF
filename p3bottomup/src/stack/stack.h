/**
 * @file stack.h
 * @brief Fully encapsulated dynamic stack for the shift/reduce automaton.
 */

#ifndef STACK_H
#define STACK_H

typedef struct {
    int state;
    int symbol;   /* symbol id (or -1 for the initial bottom marker) */
} StackItem;

typedef struct {
    StackItem *items;
    int top;       /* index of the topmost element (-1 when empty) */
    int capacity;
} Stack;

/** Allocate a stack with the given initial capacity. */
Stack *stack_init(int capacity);

/** Push a (state, symbol) pair onto the stack. Grows if needed. */
void stack_push(Stack *stack, int state, int symbol);

/** Pop the top element. Aborts if the stack is empty. */
StackItem stack_pop(Stack *stack);

/** Peek at the top element without removing it. */
StackItem stack_peek(const Stack *stack);

/** Return current number of elements on the stack. */
int stack_size(const Stack *stack);

/** Return the element at a given index (0 = bottom). */
StackItem stack_get(const Stack *stack, int index);

/**
 * Print the stack contents to stdout (state, symbol id pairs).
 * Intended for quick debugging; the logger formats richer output.
 */
void stack_print(const Stack *stack);

/** Free all memory owned by the stack. */
void stack_destroy(Stack *stack);

#endif /* STACK_H */
