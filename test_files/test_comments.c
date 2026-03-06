/*
 * Test File: Comment Removal
 * This file tests both single-line and multi-line comment removal
 */

// This is a single-line comment that should be removed
int main() { // Comment at end of line
    /* This is a block comment */
    int x = 5; /* inline block comment */ int y = 10;
    
    /* 
     * Multi-line block comment
     * This should all be removed
     * Including this line
     */
    
    // Another single line comment
    printf("Hello World"); // Print statement
    
    /* Block comment on single line */
    
    /*
    Nested-looking comment
    /* but C doesn't support true nesting */
    
    // Comment with special chars: // /* */ #define
    
    int z = /* comment in expression */ 42;
    
    return 0; // Final comment
}

// Final line comment
