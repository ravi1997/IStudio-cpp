#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"
BUILD_DIR="$ROOT_DIR/build"

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DISTUDIO_BUILD_IPL_CLI=OFF
cmake --build "$BUILD_DIR"
"$BUILD_DIR/IStudio" --lex-ipl-samples
