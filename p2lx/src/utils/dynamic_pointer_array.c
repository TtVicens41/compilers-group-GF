/**
 * Dynamic Array of Generic Pointers
 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/13
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./dynamic_pointer_array.h"

struct DynamicPointerArray {
    void **elements;
    int num_elements;
};

DynamicPointerArray *init_dynamic_pointer_array(void) {
    return calloc(1, sizeof(DynamicPointerArray));
}

void soft_clear_dynamic_pointer_array(DynamicPointerArray *array) {
    free(array->elements);
    array->elements = NULL;
    array->num_elements = 0;
}

void soft_delete_dynamic_pointer_array(DynamicPointerArray **array) {
    if (!*array) { return; }
    free(*array);
    *array = NULL;
}

void hard_clear_dynamic_pointer_array(DynamicPointerArray *array, void (*deleter)(void **)) {
    if (!array) { return; }
    for (size_t i = 0; i < array->num_elements; i++) {
        deleter(&(array->elements[i]));
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

    void **elements = array->elements;
    size_t num_elements = array->num_elements;

    elements = (void **)realloc(elements, (num_elements + 1) * sizeof(void *));
    if (!elements) { return; }

    elements[num_elements] = pointer;
    num_elements++;

    array->elements = elements;
    array->num_elements = num_elements;
}

void *get_pointer_from_array(const DynamicPointerArray *array, long position) {
    if (!array) { return NULL; }
    if (!array->elements) { return NULL; }

    void **elements = array->elements;
    size_t num_elements = array->num_elements;

    position = (num_elements + position) % num_elements;
    return elements[position];
}

size_t get_num_pointers(const DynamicPointerArray *array) {
    return array->num_elements;
}

void print_dynamic_pointer_array(const DynamicPointerArray *array) {
    if (!array) { return; }

    size_t num_elements = array->num_elements;
    void **elements = array->elements;

    if (!elements) { 
        printf("[ ]\n");
        return; 
    }

    printf("[\n");
    for (size_t i = 0; i < num_elements; i++) {
        printf("%p\n", elements[i]);
    }
    printf("]\n");
}

void print_pointer_values(const DynamicPointerArray *array, void (*printer)(void *)) {
    if (!array) { return; }
    if (!array->elements) { return; }

    void **elements = array->elements;
    size_t num_elements = array->num_elements;

    for (size_t i = 0; i < num_elements; i++) {
        printer(elements[i]);
    }
}
