/**
 * Char Map Module 
 * 
 * @description: 
 *      A char map has static number of elements, defined by ASCII_SIZE. Each 
 *      position of the char map represents a character, and each value in each
 *      position represents an assigned integer to the character. By default,
 *      each character is assigned to KEY_ERROR.
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/02/08
 */

#ifndef CHAR_MAP_H
#define CHAR_MAP_H

#define ASCII_SIZE          128
#define KEY_ERROR           (-1)
#define NUMBERS             "[0-9]"
#define NUMBERS_LETTERS     "[0-9a-zA-Z]"
#define LETTERS             "[a-zA-Z]"
#define UPPER_CASE          "[A-Z]"
#define LOWER_CASE          "[a-z]"
#define OTHER               "OTHER"
#define SPACE               "SPACE"
#define SYMBOL_SEPARATION   ' '

char *empty_char_map(void);
char *init_char_map(const char *string, const char *category);
char *init_char_map_str(const char *string);
void set_range(char *map, const char *value, char start, char end);
void set_characters(char *map, char *value, const char *characters);
void print_char_map(const char *map);
int compute_num_assigned_chars(const char *map);

#endif