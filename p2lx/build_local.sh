#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/src"
OUT_BIN="$SCRIPT_DIR/lexer"
OUT_REAL_BIN="$SCRIPT_DIR/lexer.bin"
EXTRA_CFLAGS="${EXTRA_CFLAGS:-}"

printf 'Building C Lexical Analyzer...\n'

cc -std=c11 -Wall -Wextra -pedantic \
  $EXTRA_CFLAGS \
  -I"$SRC_DIR" \
  "$SRC_DIR/automata/automata.c" \
  "$SRC_DIR/char_map/char_map.c" \
  "$SRC_DIR/token.c" \
  "$SRC_DIR/counter.c" \
  "$SRC_DIR/lexer.c" \
  "$SRC_DIR/utils/error.c" \
  "$SRC_DIR/utils/error_utils.c" \
  "$SRC_DIR/utils/string_utils.c" \
  "$SRC_DIR/utils/string_list.c" \
  "$SRC_DIR/utils/file_utils.c" \
  "$SRC_DIR/main.c" \
  -o "$OUT_REAL_BIN"

chmod +x "$OUT_REAL_BIN"

cat > "$OUT_BIN" <<'WRAPPER'
#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
if [[ ! -x "$SCRIPT_DIR/lexer.bin" ]]; then
  echo "No built lexer binary found. Run ./build_local.sh first." >&2
  exit 1
fi

exec "$SCRIPT_DIR/lexer.bin" "$@"
WRAPPER

chmod +x "$OUT_BIN"

printf 'Build successful!\n'
printf 'Executable: %s\n' "$OUT_BIN"
printf 'Binary: %s\n' "$OUT_REAL_BIN"
printf 'Usage: %s <input_file.c>\n' "$OUT_BIN"
