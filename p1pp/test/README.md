# Test Files for C Preprocessor

This directory contains comprehensive test files for validating all preprocessor functionalities.

## Test Files Overview

### 1. test_comments.c
**Purpose:** Tests comment removal functionality

**Features tested:**
- Single-line comments (`//`)
- Multi-line block comments (`/* */`)
- Comments at end of lines
- Inline comments within code
- Comments with special characters
- Comments within expressions

**Expected behavior:** All comments should be completely removed from the output.

---

### 2. test_define.c
**Purpose:** Tests `#define` directive and macro replacement

**Features tested:**
- Simple constant definitions
- String literal definitions
- Numeric constants
- Multiple uses of the same define
- Defines with underscores and numbers
- Macro replacement in expressions
- Macro replacement in array sizes

**Expected behavior:** 
- `#define` directives should be processed
- All occurrences of defined identifiers should be replaced with their values
- Identifiers used before definition should remain unchanged

---

### 3. test_ifdef.c
**Purpose:** Tests conditional compilation with `#ifdef` and `#endif`

**Features tested:**
- Basic `#ifdef` blocks with defined symbols
- `#ifdef` blocks with undefined symbols (should skip content)
- Nested `#ifdef` blocks
- Multiple separate `#ifdef` sections
- Conditional compilation around different code structures (functions, structs, typedefs)

**Expected behavior:**
- Code within `#ifdef DEFINED_SYMBOL` should be included
- Code within `#ifdef UNDEFINED_SYMBOL` should be excluded
- Proper nesting support
- Correct restoration of output state after `#endif`

---

### 4. test_include.c
**Purpose:** Tests `#include` directive for local file inclusion

**Features tested:**
- Including multiple header files
- Using definitions from included files
- Using type definitions from included files
- Using function declarations from included files

**Header files used:**
- `utils.h` - Utility definitions and function declarations
- `config.h` - Configuration constants and type definitions
- `constants.h` - Mathematical and boolean constants

**Expected behavior:**
- Header file contents should be inserted at the `#include` location
- Definitions from included files should be available for use
- Duplicate inclusion guards should prevent multiple inclusions

---

### 5. test_comprehensive.c
**Purpose:** Comprehensive integration test combining all features

**Features tested:**
- Comment removal (both types)
- `#define` directives (local)
- `#include` directives
- `#ifdef`/`#endif` conditional compilation
- Nested conditionals
- Mixing local and included definitions
- Comments within conditional blocks

**Expected behavior:**
- All features should work together seamlessly
- Include files should be processed first
- Defines from both local file and includes should work
- Comments should be removed throughout
- Conditional blocks should be properly processed

---

## Header Files

### utils.h
- Utility version constant
- Buffer size constant
- Function declarations

### config.h
- Application name and version
- Debug level setting
- Dimensions structure definition

### constants.h
- Mathematical constants (PI, E, GOLDEN_RATIO)
- Boolean constants (TRUE, FALSE)

---

## Running Tests

To test a specific file:
```bash
./preprocessor test_files/test_comments.c
./preprocessor test_files/test_define.c
./preprocessor test_files/test_ifdef.c
./preprocessor test_files/test_include.c
./preprocessor test_files/test_comprehensive.c
```

## Expected Outputs

Each test should produce an `output_pp.c` file with:
- All comments removed
- All `#define` symbols replaced
- Conditional blocks properly included/excluded
- Include files properly expanded
- Clean, preprocessed C code ready for compilation

## Verification

For each test file, verify:
1. No comments remain in output
2. All defined symbols are replaced with their values
3. Undefined `#ifdef` blocks are completely absent
4. Defined `#ifdef` blocks are present
5. Include file contents are properly inserted
6. Code structure and logic remain intact
