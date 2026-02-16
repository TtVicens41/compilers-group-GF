/**
 * @title: dynamic_array.h
 * @authors: Marc Bosch
 * @creation: 16/02/2026
 */


#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

typedef struct DynamicArray DynamicArray;
DynamicArray *init_dynamic_array(size_t element_size);
void clear_dynamic_array(DynamicArray *dynamic_array);
void delete_dynamic_array(DynamicArray **dynamic_array);
void append_value_to_array(DynamicArray *dynamic_array, void *value);
void *get_value_from_array(const DynamicArray *dynamic_array, long position);
size_t get_num_values(const DynamicArray *dynamic_array);
size_t get_value_size(const DynamicArray *dynamic_array);
void print_dynamic_array(const DynamicArray *dynamic_array, void (*printer)(void *));

void print_integer(void *integer);

#endif // DYNAMIC_ARRAY_H
