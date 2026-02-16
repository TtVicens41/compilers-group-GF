#!/bin/bash
# Build script for C Lexical Analyzer
# Usage: ./build.sh <debug> <local>
# - debug: 0 or 1
# - local: 0 or 1

cd src; 
if [ $? -ne 0 ]; then exit 1; fi

gcc -I. \
    -DDEBUG=$1 \
    -DLOCAL=$2 \
    parse_arguments/parse_arguments.c \
    automata/dfa.c \
    automata/nfa.c \
    char_map/char_map.c \
    scanner/token.c \
    scanner/lexer.c \
    scanner/scanner.c \
    utils/error_utils.c \
    utils/string_utils.c \
    utils/string_list.c \
    utils/file_utils.c \
    utils/dynamic_array.c \
    utils/dynamic_pointer_array.c \
    utils/integer_utils.c \
    main.c \
    -o \
    ../lexer
if [ $? -ne 0 ]; then exit 1; fi

chmod +x ../lexer
if [ $? -ne 0 ]; then exit 1; fi

cd ..
if [ $? -ne 0 ]; then exit 1; fi