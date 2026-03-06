/**
 * @file dynamic_value_array.h
 * @brief Dynamic Array of Generic Data Type Values
 * 
 * A dynamic array which stores values of the same size, where its size is
 * defined when creating the dynamic array. 
 * 
 * The array stores the copies for the values that are inserted to it. For 
 * example, if it wants to store an integer x = 10, the pointer passed 
 * to append function should be &x, and the value 10 will be stored to the 
 * dynamic array at its last available position. 
 * 
 * The dynamic array membres cannot be accessed outside the implementation, 
 * unless the functions or procedures of this module are called over a dynamic 
 * array.
 * 
 * @warning The programmer must keep track of what kind of data type is stored 
 * into this array.
 * 
 * @author Marc Bosch Manzano
 * @since 2026-02-12
 */

#ifndef DYNAMIC_VALUE_ARRAY_H
#define DYNAMIC_VALUE_ARRAY_H

#include <stddef.h>

/**
 * @brief A contiguous dynamic array of values (opaque).
 */
typedef struct DynamicValueArray DynamicValueArray;

/**
 * @brief Initializes the dynamic value array by setting the size of its 
 * values.
 * 
 * @param value_size The size of a data type, e.g. sizeof(int) for the case
 * of an integer array.
 * @return A dynamically allocated dynamic value array. 
 */
DynamicValueArray *init_dynamic_value_array(size_t value_size);

/**
 * @brief Clears the values inside a dynamic value array without deleting 
 * the array itself.
 * @param array A pointer to a dynamic value array.
 */
void clear_dynamic_value_array(DynamicValueArray *array);

/**
 * @brief Deletes the dynamic value array, deleting it and its values, 
 * and setting it to NULL.
 * 
 * @param array A pointer to a pointer of a dynamic value array.
 */
void delete_dynamic_value_array(DynamicValueArray **array);

/**
 * @brief Appends a value to the last available position of the dynamic
 * value array.
 * 
 * A copy of the value is appended to the dynamic value array. If value is 
 * associated to a data structure that stores pointers, only the pointer 
 * references are copied to the dynamic value array, whithout including 
 * the derreferences of these pointers.
 * 
 * @param array A pointer to a dynamic value array.
 * @param value A pointer to a value to be appended and copied to the array.
 */
void append_value_to_array(DynamicValueArray *array, const void *value);

/**
 * @brief Get a value from a specific position of the array.
 * 
 * @param array A pointer to a read-only dynamic value array.
 * @param position A position to obtain the value. It can be any integer
 * (positive or negative) and it is modularized to the range `0` to `n - 1`, 
 * where `n` is the current number of values of the array.
 * 
 * @return A pointer to a value in the array, which has to be casted to the 
 * adequate data type.
 */
const void *get_value_from_array(const DynamicValueArray *array, long position);

/**
 * @brief Get the current number of values stored in array.
 * @param array A pointer to a read-only dynamic value array.
 * @return The number of stored values.
 */
size_t get_num_values_from_array(const DynamicValueArray *array);

/**
 * @brief Get the data type size of values stored in array.
 * @param array A pointer to a read-only dynamic value array.
 * @return The ata type size of values.
 */
size_t get_value_size_from_array(const DynamicValueArray *array);

/**
 * @brief Prints the values of the array with a given printer function.
 * 
 * The values that are contained by the array and the values that accept
 * the printer function have to be from the same data type.
 * 
 * @param array A pointer to a read-only dynamic value array.
 * @param printer A function that accepts a pointer to the value and prints
 * to the standard output the value of a formatted way.
 */
void print_dynamic_value_array(
    const DynamicValueArray *array, 
    void (*printer)(const void *)
);

#endif // DYNAMIC_ARRAY_H
