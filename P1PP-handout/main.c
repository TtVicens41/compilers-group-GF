// Title: Preprocessor Main File
// Brief: A file that reads the arguments of console
// Author: Marc Bosch Manzano
// Creation: 09/01/2026
 
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    printf("%d\n", argc);

    int comments = 1;
    int defines = 0;
    int all = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i] == "-c") {
            comments = 1;
        }
        if (argv[i] == )
        printf("%s\n", argv[i]);
    }

    return 0;
}