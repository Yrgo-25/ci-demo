#!/usr/bin/env bash
#
# Build the firmware.
#
# Run via 'make build', or directly:
#   ci/build.sh
set -euo pipefail

# Root directory, resolved to an absolute path so that the script can be run from anywhere.
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

# Firmware target.
TARGET="firmware"

# C++ compiler.
CXX_COMPILER="g++"

# C++ compiler flags.
CXX_FLAGS=(-Wall -Werror -std=c++17 -Iinclude)

# Source files - update this list as new source files are added to the firmware.
SRC_FILES=(source/main.cpp)

# Build the firmware.
"$CXX_COMPILER" "${SRC_FILES[@]}" -o "$TARGET" "${CXX_FLAGS[@]}"
