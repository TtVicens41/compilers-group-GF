/**
 * @file dynamic_value_array.c
 * @brief Dynamic Array of Generic Data Type Values
 * @author Marc Bosch Manzano
 * @since 2026-02-11
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./dynamic_value_array.h"

#define CHUNK_SIZE 256  /* Number of bytes per chunk */

struct DynamicValueArray {
    void *values;       /* A generic buffer to store values. */
    size_t num_values;  /* The number of values that it stores the array. */
    size_t value_size;  /* The size in bytes of the data type of the values. */
    size_t chunk_size;  /* The chunk size in bytes for memory reallocations. */
};

DynamicValueArray *init_dynamic_value_array(size_t value_size) {
    DynamicValueArray *array = calloc(1, sizeof(DynamicValueArray));
    if (!array) { return NULL; }
    array->value_size = value_size;
    array->chunk_size = CHUNK_SIZE;
    return array;
}

void clear_dynamic_value_array(DynamicValueArray *array) {
    if (!array) { return; }
    free(array->values);
    array->values = NULL;
    array->num_values = 0;
}

void delete_dynamic_value_array(DynamicValueArray **array) {
    if (!*array) { return; }
    clear_dynamic_value_array(*array);
    free(*array);
    *array = NULL;
}

void append_value_to_array(DynamicValueArray *array, const void *value) {
    if (!array || !value) { return; }
    
    void *values = array->values;
    size_t num_values = array->num_values;
    const size_t value_size = array->value_size;
    const size_t chunk_size = array->chunk_size;

    if ((num_values % chunk_size) == 0) {
        size_t new_size;
        new_size = (num_values + 1) * chunk_size * value_size; 

        values = (void *)realloc(values, new_size);
        if (!values) { return; }
    }
    
    void *offset = values + num_values * value_size;
    memcpy(offset, value, value_size);

    array->num_values++;
    array->values = values;
}

const void *get_value_from_array(const DynamicValueArray *array, long position) {
    if (!array) { return NULL; }
    if (!array->values) { return NULL; }

    void *values = array->values;
    size_t num_values = array->num_values;
    size_t value_size = array->value_size;

    position = (num_values + position) % num_values;
    return values + position * value_size;
}

size_t get_num_values_from_array(const DynamicValueArray *array) {
    return array->num_values;
}

size_t get_value_size_from_array(const DynamicValueArray *array) {
    return array->value_size;
}

void print_dynamic_value_array(const DynamicValueArray *array, void (*printer)(const void *)) {
    if (!array) { return; }
    if (!array->values) { return; }

    void *values = array->values;
    size_t num_values = array->num_values;
    size_t value_size = array->value_size;

    for (size_t i = 0; i < num_values; i++) {
        printer(values + i * value_size);
    }
}

#if (DYNAMIC_VALUE_ARRAY_C == 1)
/** gcc -DDYNAMIC_VALUE_ARRAY_C=1 dynamic_value_array.c */

void print_integer(const int *integer) {
    printf("%d ", *integer);
}

int main(void) {
    int x = 0;

    DynamicValueArray *array = init_dynamic_value_array(sizeof(int));
    
    append_value_to_array(array, &x); x++;
    append_value_to_array(array, &x); x++;
    append_value_to_array(array, &x); x++;
    append_value_to_array(array, &x); x++;
    append_value_to_array(array, &x); x++;

    print_dynamic_value_array(array, (void (*)(const void *))print_integer);

    printf("\n%d\n", *(int *)get_value_from_array(array, 1));
    return 0;
}
#endif
