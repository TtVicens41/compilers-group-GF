/**
 * Dynamic Array of Generic Pointers
 * 
 * @description: 
 *      A dynamic array which stores generic memory addresses, notated as 
 *      `void *`. The memory addresses can be of any type of data structure,
 *      but it is recommended that all addresses point to the same type of data 
 *      structure, since the dynamic pointer array does not handle the kind of 
 *      pointers it stores. The pointers the pointer arry stores can be freed 
 *      outside it, and it does not knows which of them were freed. The way 
 *      pointers is consecutively in a buffer.
 * 
 * @structs:
 *      01      DynamicPointerArray (opaque)
 * 
 * @functions:
 *      01      init_dynamic_pointer_array
 *      02      soft_clear_dynamic_pointer_array
 *      03      soft_delete_dynamic_pointer_array
 *      04      hard_clear_dynamic_pointer_array  
 *      05      hard_delete_dynamic_pointer_array
 *      06      append_pointer_to_array
 *      07      get_pointer_from_array
 *      08      get_num_pointers
 *      09      print_dynamic_pointer_array
 *      10      print_pointer_values
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/13
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