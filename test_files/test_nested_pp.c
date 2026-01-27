

#define LEVEL_1
#define LEVEL_2
#define LEVEL_3

int global = 1;

#ifdef LEVEL_1
int level1_var = 10;

        #ifdef LEVEL_2
    int level2_var = 20;
    
                #ifdef LEVEL_3
        int level3_var = 30;
        #endif
        
    #endif
    
#endif

#ifdef LEVEL_1
int l1_defined = 1;

    #ifdef UNDEFINED_A
    int should_not_appear_a = 0;
    
        #ifdef LEVEL_2
        int should_not_appear_b = 0;
        #endif
        
    #endif
    
    #ifdef LEVEL_2
    int l2_defined = 1;
    
        #ifdef UNDEFINED_B
        int should_not_appear_c = 0;
        #endif
        
    #endif
    
#endif

#ifdef DEFINED_X
int x = 1;
#endif

#ifdef UNDEFINED_Y
int y = 2;
#endif

#ifdef DEFINED_Z
int z = 3;
#endif

#ifdef UNDEFINED_W
int w = 4;
#endif

#define NEW_SYMBOL 42

#ifdef NEW_SYMBOL
int new_val = NEW_SYMBOL;
#endif

int main() {
        #ifdef LEVEL_1
    printf("L1: defined");
    
        #ifdef LEVEL_2
        printf("L1+L2: both defined");
        
            #ifdef LEVEL_3
            printf("All three levels active");
            #endif
            
        printf("Levels 1 and 2 active");
        #endif
        
    printf("Level 1 active");
    #endif
    
    #ifdef LEVEL_1
        #ifdef LEVEL_2
            #ifdef LEVEL_3
            int deeply_nested = 100;
            printf("Deep nesting works");
            #endif
        #endif
    #endif
    
    return 0;
}
