/**
 * @title: counter.c
 * @authors: Joan Vicente, Davi Paiva
 * @creation:
 */

#include <stdio.h>
#include <string.h>

#include "counter.h"

/**
 * @brief Describes the responsibility of `should_count` in the compiler pipeline.
 * @param Receives: `const CounterState *counter`.
 * @return Returns a value of type `static int`.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
static int should_count(const CounterState *counter) {
    return counter && counter->enabled;
}

static CounterFunctionRow *get_row(CounterState *counter, const char *func) {
    int i;
    int free_idx = -1;

    for (i = 0; i < (int)(sizeof(counter->rows) / sizeof(counter->rows[0])); i++) {
        if (counter->rows[i].used) {
            if (strcmp(counter->rows[i].name, func) == 0) {
                return &counter->rows[i];
            }
        } else if (free_idx < 0) {
            free_idx = i;
        }
    }

    if (free_idx < 0) {
        return NULL;
    }

    counter->rows[free_idx].name = func;
    counter->rows[free_idx].comp = 0;
    counter->rows[free_idx].io = 0;
    counter->rows[free_idx].gen = 0;
    counter->rows[free_idx].used = 1;
    return &counter->rows[free_idx];
}

/**
 * @brief Describes the responsibility of `counter_init` in the compiler pipeline.
 * @param Receives: `CounterState *counter, FILE *stream, int enabled`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void counter_init(CounterState *counter, FILE *stream, int enabled) {
    int i;
    if (!counter) {
        return;
    }

    counter->total_comp = 0;
    counter->total_io = 0;
    counter->total_gen = 0;
    counter->stream = stream;
    counter->enabled = enabled;

    for (i = 0; i < (int)(sizeof(counter->rows) / sizeof(counter->rows[0])); i++) {
        counter->rows[i].name = NULL;
        counter->rows[i].comp = 0;
        counter->rows[i].io = 0;
        counter->rows[i].gen = 0;
        counter->rows[i].used = 0;
    }
}

/**
 * @brief Describes the responsibility of `counter_set_stream` in the compiler pipeline.
 * @param Receives: `CounterState *counter, FILE *stream`.
 * @return Does not return a value.
 * @details Performs a focused task to keep the code modular and easier to maintain.
 */
void counter_set_stream(CounterState *counter, FILE *stream) {
    if (!counter) {
        return;
    }
    counter->stream = stream;
}

static void counter_write(CounterState *counter, int line, const char *func,
                          long inc_comp, long inc_io, long inc_gen) {
    CounterFunctionRow *row;

    if (!should_count(counter) || !counter->stream) {
        return;
    }

    row = get_row(counter, func);
    if (!row) {
        return;
    }

    row->comp += inc_comp;
    row->io += inc_io;
    row->gen += inc_gen;

    counter->total_comp += inc_comp;
    counter->total_io += inc_io;
    counter->total_gen += inc_gen;

    fprintf(counter->stream,
            "[COUNT] line=%d func=%s inc={comp:%ld,io:%ld,gen:%ld} "
            "partial={comp:%ld,io:%ld,gen:%ld} "
            "total={comp:%ld,io:%ld,gen:%ld}\n",
            line, func, inc_comp, inc_io, inc_gen, row->comp, row->io, row->gen,
            counter->total_comp, counter->total_io, counter->total_gen);
}

void counter_add_comp(CounterState *counter, int line, const char *func,
                      long amount) {
    counter_write(counter, line, func, amount, 0, 0);
}

void counter_add_io(CounterState *counter, int line, const char *func,
                    long amount) {
    counter_write(counter, line, func, 0, amount, 0);
}

void counter_add_gen(CounterState *counter, int line, const char *func,
                     long amount) {
    counter_write(counter, line, func, 0, 0, amount);
}
