/**
 * @title: dynamic_array.c
 * @authors: Marc Bosch
 * @creation: 16/02/2026
 */


#include "dynamic_array.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct DynamicArray {
    void *elements;
    size_t num_elements;
    size_t element_size;
};

DynamicArray *init_dynamic_array(size_t element_size) {
    DynamicArray *dynamic_array = calloc(1, sizeof(DynamicArray));
    if (!dynamic_array) { 
        return NULL; 
    }
    dynamic_array->element_size = element_size;
    return dynamic_array;
}

void append_value_to_array(DynamicArray *dynamic_array, void *element) {
    if (!dynamic_array || !element) { return; }
    
    void *elements = dynamic_array->elements;
    size_t num_elements = dynamic_array->num_elements;
    size_t element_size = dynamic_array->element_size;

    elements = realloc(elements, (num_elements + 1) * element_size);
    if (!elements) { return; }
    
    void *offset = elements + num_elements * element_size;
    memcpy(offset, element, element_size);

    dynamic_array->num_elements++;
    dynamic_array->elements = elements;
}

void *get_value_from_array(const DynamicArray *dynamic_array, long position) {
    if (!dynamic_array) { return NULL; }
    if (!dynamic_array->elements) { return NULL; }

    void *elements = dynamic_array->elements;
    size_t num_elements = dynamic_array->num_elements;
    size_t element_size = dynamic_array->element_size;

    position = (num_elements + position) % num_elements;
    return elements + position * element_size;
}

size_t get_num_values(const DynamicArray *dynamic_array) {
    return dynamic_array->num_elements;
}

size_t get_value_size(const DynamicArray *dynamic_array) {
    return dynamic_array->element_size;
}

void print_dynamic_array(const DynamicArray *dynamic_array, void (*printer)(void *)) {
    if (!dynamic_array) { return; }
    if (!dynamic_array->elements) { return; }

    void *elements = dynamic_array->elements;
    size_t num_elements = dynamic_array->num_elements;
    size_t element_size = dynamic_array->element_size;

    for (size_t i = 0; i < num_elements; i++) {
        printer(elements + i * element_size);
    }
}

void print_integer(void *integer) {
    printf("%d ", *((int *)integer));
}

void clear_dynamic_array(DynamicArray *dynamic_array) {
    if (!dynamic_array) { return; }
    free(dynamic_array->elements);
    dynamic_array->elements = NULL;
    dynamic_array->num_elements = 0;
}

void delete_dynamic_array(DynamicArray **dynamic_array) {
    if (!*dynamic_array) { return; }
    clear_dynamic_array(*dynamic_array);
    free(*dynamic_array);
    *dynamic_array = NULL;
}

#if (0)
int main(void) {
    int x = 0;

    DynamicArray *array1 = init_dynamic_array(sizeof(int));
    
    append_value_dynamic_array(array1, &x); x++;
    append_value_dynamic_array(array1, &x); x++;
    append_value_dynamic_array(array1, &x); x++;
    append_value_dynamic_array(array1, &x); x++;
    append_value_dynamic_array(array1, &x); x++;

    print_dynamic_array(array1, print_integer);

    printf("\n%d\n", *(int *)get_value_dynamic_array(array1, 0));

    DynamicPointerArray *array2 = init_dynamic_pointer_array();

    append_pointer_to_array(array2, &x);
    append_pointer_to_array(array2, &x);
    append_pointer_to_array(array2, &x);
    append_pointer_to_array(array2, &x);

    print_dynamic_pointer_array(array2);

    printf("\n%p\n", get_pointer_from_array(array2, 0));
    return 0;
}
#endif
