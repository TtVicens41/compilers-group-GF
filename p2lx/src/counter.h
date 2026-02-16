/**
 * Counters for Token Operations
 * 
 * @author: Marc Bosch Manzano
 * @creation: 2026/01/30
 */

#ifndef COUNTER_H
#define COUNTER_H

#define DBGCOUNT 0
#define OUT 1
#define COUNTOUT OUT 
#define COUNFTILE "%s.%sdbgcnt"

/// @brief Counter for counting comparison operations.
static int countcomp = 0;

/// @brief Counter for counting input/output operations.
static int countio = 0;

/// @brief Counter for counting generic operations.
static int countgen = 0;

#endif
