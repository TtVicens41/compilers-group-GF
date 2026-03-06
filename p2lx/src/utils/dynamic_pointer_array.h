/**
 * @file dynamic_pointer_array.h
 * @brief Dynamic Array of Generic Pointers
 *
 * A dynamic array which stores generic memory addresses, aka pointers, 
 * notated as `void *`. The way pointers are stored is consecutively in a
 * buffer. The pointers can be of any type of data structure, but it is 
 * recommended to keep all pointers of the same type.
 * 
 * @warning (1) The programmer must keep track of what kind of pointers are 
 * stored into this array. (2) It can store pointers which were already freed. 
 * (3) It can stores stack memory pointers, which cannot be deleted, but 
 * cleared. (4) It can stores several times the same pointer. (5) It can 
 * store NULL pointers.
 * 
 * @author Marc Bosch Manzano
 * @since 2026-02-13
 */

#ifndef DYNAMIC_POINTER_ARRAY_H
#define DYNAMIC_POINTER_ARRAY_H

#include <stddef.h>

/**
 * @brief A contiguous dynamic array of pointers (opaque).
 */
typedef struct DynamicPointerArray DynamicPointerArray;

/**
 * @brief Initializes the dynamic pointer array.
 * @return A dynamically allocated dynamic pointer array. 
 */
DynamicPointerArray *init_dynamic_pointer_array(void);

/**
 * @brief Clears the conatiner of pointers of the array without deleting 
 * array itself nor the pointers.
 * @param array A pointer to dynamic pointer array.
 */
void soft_clear_dynamic_pointer_array(DynamicPointerArray *array);

/**
 * @brief Deletes the array without deleting the pointers it stores.
 * @param array A pointer to a pointer of an dynamic pointer array.
 */
void soft_delete_dynamic_pointer_array(DynamicPointerArray **array);

/**
 * @brief Deletes the pointers stored by the array without deleting the array
 * itself.
 * @param array A pointer to an array storing pointers to the same data type.
 * @param deleter A function applied to each pointer to delete it.
 */
void hard_clear_dynamic_pointer_array(
    DynamicPointerArray *array, 
    void (*deleter)(void **)
);

/**
 * @brief Deletes the array including the deletion of the pointers it stores.
 * @param array A pointer to a pointer of an array storing pointers to the same
 * data type.
 * @param deleter A function applied to each pointer to delete it.
 */
void hard_delete_dynamic_pointer_array(
    DynamicPointerArray **array, 
    void (*deleter)(void **)
);

/**
 * @brief Appends a pointer to the last available position of the dynamic
 * value array.
 * @param array A pointer to a dynamic value array.
 * @param pointer A pointer to be appended and copied to the array.
 */
void append_pointer_to_array(DynamicPointerArray *array, void *pointer);

/**
 * @brief Get a pointer from a specific position of the array.
 * 
 * @param array A pointer to a read-only dynamic pointer array.
 * @param position A position to obtain the pointer. It can be any integer
 * (positive or negative) and it is modularized to the range `0` to `n - 1`, 
 * where `n` is the current number of pointers of the array.
 * 
 * @return A pointer from the array.
 */
void *get_pointer_from_array(const DynamicPointerArray *array, long position);

/**
 * @brief Get the number of pointers stored in array.
 * @param array A pointer to a read-only dynamic pointer array.
 * @return The number of stored pointers.
 */
size_t get_num_pointers(const DynamicPointerArray *array);

/**
 * @brief Prints a list of raw pointer addresses of the array.
 * @param array A pointer to a read-only dynamic value array.
 */
void print_dynamic_pointer_array(const DynamicPointerArray *array);

/**
 * @brief Prints the pointers of the array with a given printer function.
 * @param array A pointer to a read-only dynamic pointer array storing pointers
 * to the same data type.
 * @param printer A function that accepts a pointer prints it to the standard
 * output in a formatted way.
 */
void print_pointer_values(
    const DynamicPointerArray *array, 
    void (*printer)(void *)
);

#endif // DYNAMIC_POINTER_ARRAY_H
