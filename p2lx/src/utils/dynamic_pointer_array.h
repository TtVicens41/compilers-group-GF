/**
 * @title: dynamic_pointer_array.h
 * @authors: Joan Vicente
 * @creation: 16/02/2026
 */


#ifndef DYNAMIC_POINTER_ARRAY_H
#define DYNAMIC_POINTER_ARRAY_H

#include <stddef.h>

typedef struct DynamicPointerArray DynamicPointerArray;
DynamicPointerArray *init_dynamic_pointer_array(void);
void soft_clear_dynamic_pointer_array(DynamicPointerArray *array);
void soft_delete_dynamic_pointer_array(DynamicPointerArray **array);
void hard_clear_dynamic_pointer_array(DynamicPointerArray *array, void (*deleter)(void **));
void hard_delete_dynamic_pointer_array(DynamicPointerArray **array, void (*deleter)(void **));
void append_pointer_to_array(DynamicPointerArray *array, void *pointer);
void *get_pointer_from_array(const DynamicPointerArray *array, long position);
size_t get_num_pointers(const DynamicPointerArray *array);
void print_dynamic_pointer_array(const DynamicPointerArray *array);
void print_pointer_values(const DynamicPointerArray *array, void (*printer)(void *));

#endif // DYNAMIC_POINTER_ARRAY_H
