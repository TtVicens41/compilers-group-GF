/**
 * @title: counter.h
 * @authors:
 * @creation:
 */

#ifndef COUNTER_H
#define COUNTER_H

#include <stdio.h>

#include "lexer_config.h"

typedef struct {
    const char *name;
    long comp;
    long io;
    long gen;
    int used;
} CounterFunctionRow;

typedef struct {
    long total_comp;
    long total_io;
    long total_gen;
    CounterFunctionRow rows[128];
    FILE *stream;
    int enabled;
} CounterState;

/**
 * @brief Describes the responsibility of `counter_init` in the compiler pipeline.
 * @param Receives: `CounterState *counter, FILE *stream, int enabled`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void counter_init(CounterState *counter, FILE *stream, int enabled);
/**
 * @brief Describes the responsibility of `counter_set_stream` in the compiler pipeline.
 * @param Receives: `CounterState *counter, FILE *stream`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void counter_set_stream(CounterState *counter, FILE *stream);
void counter_add_comp(CounterState *counter, int line, const char *func,
                      long amount);
void counter_add_io(CounterState *counter, int line, const char *func,
                    long amount);
void counter_add_gen(CounterState *counter, int line, const char *func,
                     long amount);

#define COUNT_COMP(ctx, line, amount) \
    counter_add_comp((ctx), (line), __func__, (amount))
#define COUNT_IO(ctx, line, amount) \
    counter_add_io((ctx), (line), __func__, (amount))
#define COUNT_GEN(ctx, line, amount) \
    counter_add_gen((ctx), (line), __func__, (amount))

#endif
