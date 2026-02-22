/**
 * @file integer_utils.c
 * @brief Integer Array and Matrix Utilities
 * @author Marc Bosch Manzano
 * @since 2026-02-16
 */

#include "integer_utils.h"
#include "dynamic_value_array.h"
#include "dynamic_pointer_array.h"

#include <stdio.h>
#include <stdlib.h>

void print_int(const int *integer) {
    printf("%d ", *integer);
}

struct IntegerArray {
    DynamicValueArray *dynamic_array;
};

IntegerArray *init_integer_array(void) {
    IntegerArray *integer_array;
    integer_array = calloc(1, sizeof(IntegerArray));
    if (!integer_array) {
        return NULL; 
    }
    integer_array->dynamic_array = init_dynamic_value_array(sizeof(int));
    return integer_array;
}

void clear_integer_array(IntegerArray *integer_array) {
    if (!integer_array) { return; }
    delete_dynamic_value_array(&(integer_array->dynamic_array));
}

void delete_integer_array(IntegerArray **integer_array) {
    if (!*integer_array) { return; }
    clear_integer_array(*integer_array);
    free(*integer_array);
    *integer_array = NULL;
}

void append_integer_to_array(IntegerArray *integer_array, int integer) {
    if (!integer_array) { return; }
    append_value_to_array(
        integer_array->dynamic_array, 
        (void *)&integer
    );
}

int get_integer_from_array(const IntegerArray *integer_array, long position) {
    if (!integer_array) { return 0; }
    return *(int *)get_value_from_array(
        integer_array->dynamic_array, 
        position
    );
}

void print_integer_array(const IntegerArray *integer_array) {
    if (!integer_array) { 
        printf("%s", (char *)integer_array);
        return; 
    }

    printf("[ ");
    print_dynamic_value_array(
        integer_array->dynamic_array, 
        (void (*))(const void *)print_int
    );
    printf("]");
}

void print_raw_integer_array(const int *array, int size) {
    if (!array) { 
        printf("%s", (char *)array);
        return; 
    }

    printf("[ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }

    printf("]");
}

struct IntegerMatrix {
    DynamicPointerArray *dynamic_pointer_array;
};

void print_raw_integer_matrix(int **matrix, int rows, int cols) {
    if (!matrix) { 
        printf("%s", (char *)matrix);
        return; 
    }

    printf("\n[\n");
    for (int i = 0; i < rows; i++) {
        print_raw_integer_array(matrix[i], cols);
        printf("\n");
    }

    printf("]\n");
}

    
// IntegerArray *integer_array = init_integer_array();
// append_integer_to_array(integer_array, 330);
// append_integer_to_array(integer_array, 220);
// append_integer_to_array(integer_array, 110);
// append_integer_to_array(integer_array, 990);
// append_integer_to_array(integer_array, 220);
// print_integer_array(integer_array);
// clear_integer_array(integer_array);
// print_integer_array(integer_array);
// delete_integer_array(&integer_array);
// print_integer_array(integer_array);
// int x = 0;

// DynamicPointerArray *array = init_dynamic_pointer_array();
// append_pointer_to_array(array, &x);
// append_pointer_to_array(array, &x);
// append_pointer_to_array(array, &x);
// append_pointer_to_array(array, &x);

// print_dynamic_pointer_array(array);
// soft_clear_dynamic_pointer_array(array);
// print_dynamic_pointer_array(array);
// soft_delete_dynamic_pointer_array(&array);
// print_dynamic_pointer_array(array);
