#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/src"
OUT_BIN="$SCRIPT_DIR/lexer"
DEBUG_FLAG="${1:-0}"
LOCAL_FLAG="${2:-1}"

cc -I"$SRC_DIR" \
    -DDEBUG="$DEBUG_FLAG" \
    -DLOCAL="$LOCAL_FLAG" \
    "$SRC_DIR/parse_arguments/parse_arguments.c" \
    "$SRC_DIR/automata/dfa.c" \
    "$SRC_DIR/automata/nfa.c" \
    "$SRC_DIR/char_map/char_map.c" \
    "$SRC_DIR/scanner/token.c" \
    "$SRC_DIR/scanner/lexer.c" \
    "$SRC_DIR/scanner/scanner.c" \
    "$SRC_DIR/utils/error_utils.c" \
    "$SRC_DIR/utils/string_utils.c" \
    "$SRC_DIR/utils/string_list.c" \
    "$SRC_DIR/utils/file_utils.c" \
    "$SRC_DIR/utils/dynamic_array.c" \
    "$SRC_DIR/utils/dynamic_pointer_array.c" \
    "$SRC_DIR/utils/integer_utils.c" \
    "$SRC_DIR/main.c" \
    -o "$OUT_BIN"

chmod +x "$OUT_BIN"
