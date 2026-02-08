#!/bin/bash
# Build script for C Lexical Analyzer

echo "Building C Lexical Analyzer..."

cd src

gcc -I. \
    automata/automata.c \
    char_map/char_map.c \
    utils/error_utils.c \
    utils/string_utils.c \
    utils/file_utils.c \
    main.c \
    -o ../lexer

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

chmod +x ../lexer
cd ..

echo "Build successful!"
echo "Lexical Analyzer executable created: ./lexer"
echo ""
echo "Usage: ./lexer <input_file>"
echo "Example: ./lexer test_files/example.c"
echo ""
echo "Warning: This command can only be executed in this directory"
