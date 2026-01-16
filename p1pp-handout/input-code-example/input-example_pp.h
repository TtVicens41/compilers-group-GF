/*
 * Compilers
 * Input example code: Define simple .c and .h files to test your processing
 * File name: input-example.h
 * Additional files: input-example.c
 */

// Your pre-processor is not required to include c libraries. 
// Just comment them out or leave the include untouched.




// MACROS used for the printouts instead of using f/printfs so that all prints
// in the program are treated the same way.
// Use ERROR when the print out informs of a ERROR/problem in the program and it must finish the program
// Use WARNING when the print out informs of a ERROR/problem in the program but the preprocessor can continue
// You have to define the format of your errors, this is just an example to test, 
// but your code can do a different version of this to treat errors
/* usage: ERROR(("Warning: Note the two brackets\n")) */
                        printf("ERROOORRRRRRRRRR see output file\n");})

FILE    *errfile;        // File where to write error messages
FILE    *ofile;          // File where to write program information

