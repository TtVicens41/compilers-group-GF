/*
 * Test File: #include Directive
 * Tests file inclusion functionality
 */

#include "utils.h"
#include "config.h"
#include "constants.h"

int main() {
    // Test using definitions from utils.h
    int buffer_size = MAX_BUFFER;
    int version = UTILS_VERSION;
    
    // Test using definitions from config.h
    printf(APP_NAME);
    int app_ver = APP_VERSION;
    int debug = DEBUG_LEVEL;
    
    Dimensions screen;
    screen.width = 1920;
    screen.height = 1080;
    
    // Test using definitions from constants.h
    double pi = PI;
    double e = E;
    double gr = GOLDEN_RATIO;
    int true_val = TRUE;
    int false_val = FALSE;
    
    // Test function declarations from utils.h
    int sum = add(5, 3);
    int diff = subtract(10, 4);
    
    return 0;
}

// Test including after function definition
void another_function() {
    int max = MAX_BUFFER;
    printf("Buffer: %d", max);
}
