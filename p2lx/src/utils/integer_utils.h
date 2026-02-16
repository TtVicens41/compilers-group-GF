/**
 * @title: integer_utils.h
 * @authors: Joan Vicente
 * @creation: 16/02/2026
 */


#ifndef INTEGER_H
#define INTEGER_H

typedef struct IntegerArray IntegerArray;
IntegerArray *init_integer_array(void);
void clear_integer_array(IntegerArray *integer_array);
void delete_integer_array(IntegerArray **integer_array);
void append_integer_to_array(IntegerArray *integer_array, int integer);
int get_integer_from_array(const IntegerArray *integer_array, long position);
void print_integer_array(const IntegerArray *integer_array);

void print_raw_integer_array(const int *array, int size);
void print_raw_integer_matrix(int **matrix, int rows, int cols);

#endif // INTEGER_UTILS_H
