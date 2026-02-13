#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
INPUT="$SCRIPT_DIR/acceptance_input.c"
OUTPUT="$INPUT""scn"
COUNT_OUTPUT="$INPUT""dbgcnt"

fail() {
  echo "[FAIL] $1" >&2
  exit 1
}

assert_file() {
  [[ -f "$1" ]] || fail "Missing file: $1"
}

assert_contains() {
  local file="$1"
  local pattern="$2"
  grep -Fq "$pattern" "$file" || fail "Pattern not found in $file: $pattern"
}

assert_not_contains_regex() {
  local file="$1"
  local pattern="$2"
  if grep -Eq "$pattern" "$file"; then
    fail "Unexpected pattern in $file: $pattern"
  fi
}

rm -f "$OUTPUT" "$COUNT_OUTPUT"

echo "[1/3] Build + run DEBUG format"
EXTRA_CFLAGS='-DOUTFORMAT=OUTFORMAT_DEBUG -DDEBUG=DEBUG_OFF -DCOUNTCONFIG=COUNT_DISABLED' \
  "$PROJECT_DIR/build_local.sh"
"$PROJECT_DIR/lexer" "$INPUT" >/tmp/p2_accept_debug_stdout.txt 2>&1
assert_file "$OUTPUT"
assert_contains "$OUTPUT" "<if, CAT_KEYWORD>"
assert_contains "$OUTPUT" "<while, CAT_KEYWORD>"
assert_contains "$OUTPUT" "<int, CAT_KEYWORD>"
assert_contains "$OUTPUT" "<char, CAT_KEYWORD>"
assert_contains "$OUTPUT" "<0001, CAT_NUMBER>"
assert_contains "$OUTPUT" "<vect, CAT_IDENTIFIER>"
assert_contains "$OUTPUT" "<\"ok\", CAT_LITERAL>"
assert_contains "$OUTPUT" '<bad$token, CAT_NONRECOGNIZED>'
assert_contains "$OUTPUT" "<123abc, CAT_NONRECOGNIZED>"
assert_contains "$OUTPUT" "<@@@, CAT_NONRECOGNIZED>"


echo "[2/3] Build + run RELEASE format"
EXTRA_CFLAGS='-DOUTFORMAT=OUTFORMAT_RELEASE -DDEBUG=DEBUG_OFF -DCOUNTCONFIG=COUNT_DISABLED' \
  "$PROJECT_DIR/build_local.sh"
"$PROJECT_DIR/lexer" "$INPUT" >/tmp/p2_accept_release_stdout.txt 2>&1
assert_file "$OUTPUT"
assert_not_contains_regex "$OUTPUT" '^[0-9]+ '
assert_not_contains_regex "$OUTPUT" '^$'


echo "[3/3] Build + run DEBUG+COUNT with separate count file"
EXTRA_CFLAGS='-DOUTFORMAT=OUTFORMAT_DEBUG -DDEBUG=DEBUG_OFF -DCOUNTCONFIG=COUNT_ENABLED -DCOUNTOUT=DBGCOUNT' \
  "$PROJECT_DIR/build_local.sh"
"$PROJECT_DIR/lexer" "$INPUT" >/tmp/p2_accept_count_stdout.txt 2>&1
assert_file "$COUNT_OUTPUT"
assert_contains "$COUNT_OUTPUT" "[COUNT]"
assert_contains "$COUNT_OUTPUT" "func=process_line"

echo "[PASS] Acceptance checks completed successfully."
