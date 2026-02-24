#!/bin/bash
# ──────────────────────────────────────────────────────────────────────
# build.sh – Build the p3bottomup shift/reduce parser
# ──────────────────────────────────────────────────────────────────────
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/src"
OUT_BIN="$SCRIPT_DIR/parser"

cc -Wall -Wextra -pedantic -std=c11 \
    -I"$SRC_DIR" \
    "$SRC_DIR/main.c" \
    "$SRC_DIR/parser/parser.c" \
    "$SRC_DIR/parser/sra.c" \
    "$SRC_DIR/automaton/dfa.c" \
    "$SRC_DIR/automaton/parse_table.c" \
    "$SRC_DIR/stack/stack.c" \
    "$SRC_DIR/language/language.c" \
    "$SRC_DIR/language/grammar.c" \
    "$SRC_DIR/tokens/token.c" \
    "$SRC_DIR/tokens/token_list.c" \
    "$SRC_DIR/tokens/token_loader.c" \
    "$SRC_DIR/utils/logger.c" \
    -o "$OUT_BIN"

chmod +x "$OUT_BIN"
echo "Build successful: $OUT_BIN"
