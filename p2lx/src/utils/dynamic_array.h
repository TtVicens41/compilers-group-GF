/**
 * Dynamic Array of Generic Data Type Values
 * 
 * @description: 
 *      A dynamic array which stores elements of the same size, where
 *      the value size is defined when creating the dynamic array. The array
 *      stores the derreferenced values on the given pointers. For example, if
 *      it wants to store an integer x = 10, the pointer passed to append 
 *      function should be &x, and the value 10 will be stored to the dynamic 
 *      array at its last available position. The dynamic array membres cannot 
 *      be accessed outside the implementation, unless the functions or procedures
 *      of this module are called over a dynamic array.
 * 
 * @structs:
 *      01      DynamicArray (opaque)
 * 
 * @functions:
 *      01      init_dynamic_array
 *      02      clear_dynamic_array
 *      03      delete_dynamic_array    
 *      04      append_value_to_array
 *      05      get_value_from_array
 *      06      get_num_values
 *      07      get_value_size
 *      08      print_dynamic_array
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/12
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