/*
 * Test File: Edge Cases
 * Tests unusual but valid scenarios
 */

// Test empty lines and whitespace
#define EMPTY_AFTER_THIS

#define SPACES    123

// Multiple defines on consecutive lines
#define A 1
#define B 2
#define C 3
#define D 4

// Test define with same name as value
#define VALUE VALUE_CONSTANT

// Comments next to directives
#define MAX 100 // This is max value
#ifdef DEBUG // Check if debug
int x = 1;
#endif // End debug

// Very long identifier
#define VERY_LONG_IDENTIFIER_NAME_FOR_TESTING_PURPOSES 999

// Numbers in different formats
#define DECIMAL 42
#define HEX_LIKE 0xFF
#define NEGATIVE -1

int main() {
    // Test replacement in strings (should NOT be replaced)
    char* str = "This MAX should not be replaced";
    printf("MAX is a word here");
    
    // Test replacement as part of identifier (should NOT be replaced)
    int MAXIMUM = 200; // MAXIMUM != MAX
    int MAX_VALUE = MAX; // Only MAX should be replaced
    
    // Test with no spaces
    int sum=A+B+C+D;
    
    // Empty ifdef
    #ifdef UNDEFINED
    #endif
    
    // Ifdef immediately followed by endif
    #ifdef DEFINED
    #endif
    
    return 0;
}

/* Multi-line comment with directive-like text
#define FAKE 1
#ifdef FAKE
This should all be treated as comment
#endif
*/

// Comment with directive text: #define ALSO_FAKE 2

// Test underscore edge cases
#define _LEADING 1
#define TRAILING_ 2
#define _BOTH_ 3
#define MIDDLE_UNDER 4

int values = _LEADING + TRAILING_ + _BOTH_ + MIDDLE_UNDER;
