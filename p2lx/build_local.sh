#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "Building C Lexical Analyzer..."

debug=0
local=1
"$SCRIPT_DIR/build.sh" "$debug" "$local"

echo "Build successful!"
echo "Lexical Analyzer executable created: $SCRIPT_DIR/lexer"
echo ""
echo "Usage: $SCRIPT_DIR/lexer <input_file>"
