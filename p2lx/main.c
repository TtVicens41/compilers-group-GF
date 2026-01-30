/**
 * @title: Main Program for Lexer/Scanner
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
 */

#include <stdio.h>
#include "src/error.h"

#define MAIN 0
#define NAME MAIN

static COUNT = 0;

#if (NAME == MAIN)
int main(void) {
    printf("%s\n", ERROR_MESSAGES[ERR_NON_RECOGNIZED]); COUNT++;
    printf("%s\n", ERROR_MESSAGES[ERR_UNTERMINATED_LITERAL]); COUNT++;
    printf("%s\n", ERROR_MESSAGES[ERR_IDENTIFIER_NOT_DEFINED]); COUNT++;
}
#endif


