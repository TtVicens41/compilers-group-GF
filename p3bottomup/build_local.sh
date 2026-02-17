#!/bin/bash

set -e

echo "Building P3 Bottom-Up Parser..."

cd src

gcc -I. \
    language/language.c \
    token/token.c \
    token/token_loader.c \
    stack/parse_stack.c \
    parser/parser.c \
    trace/trace.c \
    utils/file_utils.c \
    utils/string_utils.c \
    main.c \
    -o ../p3parser

cd ..
chmod +x ./p3parser

echo "Build successful: ./p3parser"
echo "Usage: ./p3parser <input.cscn> <language.txt>"
