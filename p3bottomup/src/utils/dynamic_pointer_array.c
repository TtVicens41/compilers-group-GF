/**
 * @file dynamic_pointer_array.c
 * @brief Dynamic Array of Generic Pointers
 * @author Marc Bosch Manzano
 * @since 2026-02-13
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./dynamic_pointer_array.h"

#define CHUNK_SIZE 256   /* Number of bytes per chunk */

struct DynamicPointerArray {
    void **pointers;     /* A generic buffer to store pointers. */
    size_t num_pointers; /* The number of pointers that stores the array. */
    size_t chunk_size;   /* The chunk size in bytes for memory reallocations. */
};

DynamicPointerArray *init_dynamic_pointer_array(void) {
    DynamicPointerArray *array = calloc(1, sizeof(DynamicPointerArray));
    if (!array) { return NULL; }
    array->chunk_size = CHUNK_SIZE;
    return array;
}

void soft_clear_dynamic_pointer_array(DynamicPointerArray *array) {
    if (!array) { return; }
    free(array->pointers);
    array->pointers = NULL;
    array->num_pointers = 0;
}

void soft_delete_dynamic_pointer_array(DynamicPointerArray **array) {
    if (!*array) { return; }
    free(*array);
    *array = NULL;
}

void hard_clear_dynamic_pointer_array(DynamicPointerArray *array, void (*deleter)(void **)) {
    if (!array) { return; }
    for (size_t i = 0; i < array->num_pointers; i++) {
        deleter(&(array->pointers[i]));
    }
    soft_clear_dynamic_pointer_array(array);
}

void hard_delete_dynamic_pointer_array(DynamicPointerArray **array, void (*deleter)(void **)) {
    if (!*array) { return; }
    hard_clear_dynamic_pointer_array(*array, deleter);
    soft_delete_dynamic_pointer_array(array);
}

void append_pointer_to_array(DynamicPointerArray *array, void *pointer) {
    if (!array) { return; }

    void **pointers = array->pointers;
    size_t num_pointers = array->num_pointers;
    const size_t chunk_size = array->chunk_size;

    if ((num_pointers % chunk_size) == 0) {
        size_t new_size;
        new_size = (num_pointers + 1) * chunk_size * sizeof(void *);

        pointers = (void **)realloc(pointers, new_size);
        if (!pointers) { return; }
    }

    pointers[num_pointers] = pointer;
    num_pointers++;

    array->pointers = pointers;
    array->num_pointers = num_pointers;
}

void *get_pointer_from_array(const DynamicPointerArray *array, long position) {
    if (!array) { return NULL; }
    if (!array->pointers) { return NULL; }

    void **pointers = array->pointers;
    size_t num_pointers = array->num_pointers;

    position = (num_pointers + position) % num_pointers;
    return pointers[position];
}

size_t get_num_pointers(const DynamicPointerArray *array) {
    return array->num_pointers;
}

void print_dynamic_pointer_array(const DynamicPointerArray *array) {
    if (!array) { return; }

    size_t num_pointers = array->num_pointers;
    void **pointers = array->pointers;

    if (!pointers) { 
        printf("[ ]\n");
        return; 
    }

    printf("[\n");
    for (size_t i = 0; i < num_pointers; i++) {
        printf("%p\n", pointers[i]);
    }
    printf("]\n");
}

void print_pointer_values(const DynamicPointerArray *array, void (*printer)(void *)) {
    if (!array) { return; }
    if (!array->pointers) { return; }

    void **pointers = array->pointers;
    size_t num_pointers = array->num_pointers;

    for (size_t i = 0; i < num_pointers; i++) {
        printer(pointers[i]);
    }
}

#if (DYNAMIC_POINTER_ARRAY_C == 1)
/** gcc -DDYNAMIC_POINTER_ARRAY_C=1 dynamic_pointer_array.c */
int main(void) {
    int x[] = { 1, 2, 3, 4 };

    DynamicPointerArray *array = init_dynamic_pointer_array();

    append_pointer_to_array(array, &x[0]);
    append_pointer_to_array(array, &x[1]);
    append_pointer_to_array(array, &x[2]);
    append_pointer_to_array(array, &x[3]);

    print_dynamic_pointer_array(array);

    printf("\n%p\n", get_pointer_from_array(array, 0));
    return 0;
}
#endif
