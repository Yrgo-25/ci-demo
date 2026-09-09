#!/usr/bin/env bash
#
# Build and run the unit test suite, via the makefile in the test directory.
#
# Run via 'make unit-test', or directly:
#   ci/unit_test.sh
set -euo pipefail

# Root directory, resolved to an absolute path so that the script can be run from anywhere.
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

# Test directory, i.e. the directory holding the test suite and its makefile.
TEST_DIR="test"

# Test framework directory, i.e. the git submodule holding yrgo::test.
TEST_LIB_DIR="libs/test"

# Terminate the script if the submodule hasn't been fetched, rather than failing later with a
# confusing error from the compiler.
if [[ ! -f "$TEST_LIB_DIR/Makefile" ]]
then
    echo "error: the test framework is missing. Run 'git submodule update --init --recursive'." >&2
    exit 1
fi

# Build and run the test suite. A failed test case makes the suite return non-zero, which in turn
# fails this script and the CI job running it.
make -C "$TEST_DIR"
