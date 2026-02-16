/*
 * Test File: Nested Includes
 * Tests header files that include other header files
 */

#include "extended.h"

int main() {
    // Should have access to both base.h and extended.h definitions
    int base_val = BASE_CONSTANT;
    int ext_val = EXTENDED_CONSTANT;
    
    printf(BASE_NAME);
    printf(EXTENDED_NAME);
    
    // Use types from both headers
    BaseType base_obj;
    base_obj.id = 1;
    
    ExtendedType ext_obj;
    ext_obj.base.id = 2;
    ext_obj.extra_field = 42;
    
    return 0;
}
