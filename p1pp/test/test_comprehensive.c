/*
 * Test File: Comprehensive Test
 * Tests all preprocessor functionalities together:
 * - Comment removal (single-line and multi-line)
 * - #define directives
 * - #ifdef/#endif conditional compilation
 * - #include file inclusion
 */

// Include header files
#include "utils.h"
#include "config.h"

// Define application constants
#define VERSION 200
#define MAX_USERS 50
#define TIMEOUT_MS 5000
#define WELCOME_MSG "Welcome to the application"

// Conditional compilation flags
#define ENABLE_LOGGING
#define ENABLE_STATS

/* 
 * Main application structure
 * This comment should be removed
 */

#ifdef ENABLE_LOGGING
#define LOG_LEVEL 3
void log_message(char *msg) {
    // Implementation of logging
    printf("[LOG] %s", msg); // Print to console
}
#endif

int main() {
    // Initialize application with defines from includes
    printf(APP_NAME); // Should use config.h definition
    int ver = VERSION; /* Local version */
    
    /* Check buffer size from included header */
    int buffer = MAX_BUFFER; // From utils.h
    
    // Use local defines
    int max_users = MAX_USERS;
    int timeout = TIMEOUT_MS;
    
    #ifdef ENABLE_LOGGING
    // This block should be included
    int log_level = LOG_LEVEL;
    log_message(WELCOME_MSG); // Comment at end
    #endif
    
    #ifdef ENABLE_STATS
    /* 
     * Statistics tracking enabled
     * Multi-line comment here
     */
    int user_count = 0;
    int active_sessions = 0;
    #endif
    
    // Test nested conditionals with defines
    #ifdef ENABLE_LOGGING
        #ifdef ENABLE_STATS
        // Both features enabled
        printf("Logging and stats active");
        int combined_flag = LOG_LEVEL + user_count;
        #endif
    #endif
    
    #ifdef FEATURE_NOT_DEFINED
    // This entire block should be skipped
    int should_not_appear = 999;
    printf("This line should not be in output");
    #endif
    
    /* Use included utility functions */
    int sum = add(VERSION, APP_VERSION); // Mix of local and included defines
    
    return 0; // Exit successfully
}

// Additional function with all features
void process_data() {
    #ifdef ENABLE_LOGGING
    log_message("Processing data"); /* Inline comment */
    #endif
    
    // Single line comment
    int size = MAX_BUFFER; // From include
    int count = MAX_USERS; // From local define
    
    /*
     * Process each user
     * This is a multi-line comment
     * It should be completely removed
     */
    for (int i = 0; i < count; i++) {
        // Loop body comment
        printf("User %d", i);
    }
}

#ifdef ENABLE_STATS
/* Statistics structure */
typedef struct {
    int total_requests;  // Total number of requests
    int failed_requests; /* Failed request count */
    int avg_response_time; // Average response time in ms
} Statistics;

void print_stats(Statistics *stats) {
    printf("Total: %d", stats->total_requests);
    printf("Failed: %d", stats->failed_requests); // Print failed count
}
#endif

// Final comment at end of file
