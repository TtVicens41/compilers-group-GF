#!/bin/bash

# Build script for C Preprocessor
# Compiles all source files and creates the preprocessor executable

echo "Building C Preprocessor..."

cd src

gcc -I. \
    comments/comments.c \
    define/define.c \
    directives/directives.c \
    ifdef/ifdef.c \
    include/include.c \
    keyword_dispatcher/keyword_dispatcher.c \
    macro_expander/macro_expander.c \
    macro_parser/macro_parser.c \
    parse_arguments/parse_arguments.c \
    preprocessor/preprocessor.c \
    symbol_table/symbol_table.c \
    main.c \
    -o ../preprocessor

cd ..

if [ $? -eq 0 ]; then
    chmod +x preprocessor
    echo "✓ Build successful!"
    echo "Preprocessor executable created: ./preprocessor"
    echo ""
    echo "Usage: ./preprocessor <input_file>"
    echo "Example: ./preprocessor test_files/test_comments.c"
else
    echo "✗ Build failed!"
    exit 1
fi
