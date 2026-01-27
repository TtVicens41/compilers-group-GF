/*
 * Test File: #ifdef/#endif Directive
 * Tests conditional compilation
 */

#define DEBUG
#define FEATURE_ENABLED

// Test basic ifdef
#ifdef DEBUG
int debug_mode = 1;
#endif

// Test ifdef with undefined symbol (should skip)
#ifdef UNDEFINED_SYMBOL
int this_should_not_appear = 999;
#endif

int main() {
    #ifdef DEBUG
    printf("Debug mode is ON");
    #endif
    
    #ifdef UNDEFINED_SYMBOL
    printf("This should be skipped");
    #endif
    // Normal code always appears
    int x = 10;
    
    #ifdef FEATURE_ENABLED
    printf("Feature is enabled");
    int feature_flag = 1;
    #endif
    
    // Test nested ifdef
    #ifdef DEBUG
    printf("Outer debug block");
    
        #ifdef FEATURE_ENABLED
        printf("Nested: both DEBUG and FEATURE_ENABLED");
        int nested_var = 42;
        #endif
        
    printf("Still in DEBUG block");
    #endif
    
    // Code after ifdef blocks
    int y = 20;
    
    #ifdef PRODUCTION
    printf("Production mode - should be skipped");
    int prod_flag = 1;
    #endif
    
    return 0;
}

// Test multiple ifdef sections
#ifdef DEBUG
void debug_function() {
    printf("Debug function");
}
#endif

#ifdef RELEASE
void release_function() {
    printf("This should not appear");
}
#endif

// Test ifdef around different code structures
#ifdef FEATURE_ENABLED
struct FeatureData {
    int id;
    char name[50];
};
#endif

#ifdef ADVANCED_FEATURES
typedef struct {
    int value;
} AdvancedType;
#endif
