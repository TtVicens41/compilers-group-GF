#!/bin/bash
# Build script for C Lexical Analyzer
# "lexer" executable file generated into the Linux ./local directory
# Usage: ./build_global.sh

echo "Building C Lexical Analyzer..."

debug=0
local=0
./build.sh $debug $local
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

mkdir -p ~/.local/bin
mv ./lexer ~/.local/bin/lexer
mkdir -p ~/.local/share/lexer/
cp -r resources/. ~/.local/share/lexer
source ~/.bashrc

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Lexer executable created: lexer"
    echo ""
    echo "Usage: lexer <input_file>"
    echo "Example: lexer test/test_comments_pp.c"
else
    echo "Build failed!"
    exit 1
fi
