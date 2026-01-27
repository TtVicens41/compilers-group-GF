/*
 * Test File: #define Directive
 * Tests macro definition and replacement
 */

#define PI 3.14159
#define MAX_SIZE 100
#define MIN_VALUE 0
#define GREETING "Hello, World!"

// Test simple constant replacement
int main() {
    double radius = 5.0;
    double area = PI * radius * radius;
    
    int array[MAX_SIZE];
    int min = MIN_VALUE;
    
    printf(GREETING);
    
    // Multiple uses of same define
    int x = MAX_SIZE;
    int y = MAX_SIZE + MIN_VALUE;
    
    return 0;
}

#define BUFFER_SIZE 256
#define EOF_MARKER -1

void test_function() {
    char buffer[BUFFER_SIZE];
    int status = EOF_MARKER;
    
    // Test nested usage
    int total = MAX_SIZE + BUFFER_SIZE;
    
    // Define used in expressions
    if (status == EOF_MARKER) {
        printf("End of file");
    }
}

// Test defines with numbers and underscores
#define MAX_CONNECTIONS_10 10
#define _INTERNAL_FLAG 1
#define VERSION_2_0 200

int config = MAX_CONNECTIONS_10;
int flag = _INTERNAL_FLAG;
int version = VERSION_2_0;
