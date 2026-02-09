#include <stdio.h>
#include "token.h"

void print_token(void *token) {
    printf("{\n");
    printf("category: %s,\n", (char *)((SimpleToken *)token)->category);
    printf("lexeme: %s\n", ((SimpleToken *)token)->lexeme);
    printf("}\n");
}