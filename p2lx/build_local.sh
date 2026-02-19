#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
DEBUG_FLAG=0
LOCAL_FLAG=1

echo "Building C Lexical Analyzer..."

"$SCRIPT_DIR/build.sh" "$DEBUG_FLAG" "$LOCAL_FLAG"

echo "Build successful!"
echo "Lexical Analyzer executable created: ./lexer"
echo ""
echo "Usage: ./lexer <input_file>"
