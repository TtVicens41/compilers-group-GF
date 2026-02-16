#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
RES_DIR="$SCRIPT_DIR/resources"

echo "Building C Lexical Analyzer..."

debug=0
local=0
"$SCRIPT_DIR/build.sh" "$debug" "$local"

mkdir -p ~/.local/bin
mv "$SCRIPT_DIR/lexer" ~/.local/bin/lexer
mkdir -p ~/.local/share/lexer/
cp -r "$RES_DIR/." ~/.local/share/lexer

echo "Build successful!"
echo "Lexer executable created: ~/.local/bin/lexer"
echo ""
echo "Usage: lexer <input_file>"
