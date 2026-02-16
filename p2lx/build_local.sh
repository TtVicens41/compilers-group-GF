#!/bin/bash
# Build script for C Lexical Analyzer
# "lexer" executable file generated into the same directory as this script
# Usage: ./build_local.sh

echo "Building C Lexical Analyzer..."

debug=0
local=1
./build.sh $debug $local
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build successful!"
echo "Lexical Analyzer executable created: ./lexer"
echo ""
echo "Usage: ./lexer <input_file>"
echo "Example: ./lexer test/test_comments_pp.c"
echo ""
echo "Warning: This command can only be executed in this directory."
