#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"
BUILD_DIR="$ROOT_DIR/build"
GRAMMAR_FILE="$ROOT_DIR/examples/ipl/grammar_rules.txt"
TRANSLATION_FILE="$ROOT_DIR/examples/translation_rules.txt"
VALID_DIR="$ROOT_DIR/tests/parser_valid"
INVALID_DIR="$ROOT_DIR/tests/parser_invalid"

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DISTUDIO_BUILD_IPL_CLI=OFF
cmake --build "$BUILD_DIR"

STATUS=0

run_valid() {
  local file="$1"
  if ! "$BUILD_DIR/IStudio" compile "$file" --grammar "$GRAMMAR_FILE" --translation "$TRANSLATION_FILE" > /dev/null; then
    echo "[FAIL] valid program failed: ${file#$ROOT_DIR/}" >&2
    STATUS=1
  else
    echo "[PASS] valid program: ${file#$ROOT_DIR/}"
  fi
}

run_invalid() {
  local file="$1"
  if "$BUILD_DIR/IStudio" compile "$file" --grammar "$GRAMMAR_FILE" --translation "$TRANSLATION_FILE" > /dev/null 2>&1; then
    echo "[FAIL] invalid program succeeded: ${file#$ROOT_DIR/}" >&2
    STATUS=1
  else
    echo "[PASS] invalid program rejected: ${file#$ROOT_DIR/}"
  fi
}

for file in "$VALID_DIR"/*.ipl; do
  run_valid "$file"
done

echo "---"

if [ -d "$INVALID_DIR" ]; then
  for file in "$INVALID_DIR"/*.ipl; do
    run_invalid "$file"
  done
else
  echo "No invalid sample directory found." >&2
fi

exit $STATUS
