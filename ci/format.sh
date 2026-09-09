#!/usr/bin/env bash
#
# Format the repository's C++ code with clang-format, using the .clang-format file in the
# repository root. Files under libs/ are left alone, since they belong to another repository.
#
# Run via 'make format' or 'make check-format', or directly:
#   ci/format.sh          Format the files in place.
#   ci/format.sh --check  Report unformatted files without changing anything. Used in CI.
set -euo pipefail

# Root directory, resolved to an absolute path so that the script can be run from anywhere.
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

# Terminate the script if clang-format isn't installed.
if ! command -v clang-format &> /dev/null
then
    echo "error: clang-format not found. Install it, e.g. 'sudo apt -y install clang-format'." >&2
    exit 1
fi

# Select the repository's own C++ files, i.e. everything except the submodules under libs/.
mapfile -t FILES < <(find . -path ./libs -prune -o \( -name "*.h" -o -name "*.cpp" \) -print | sort)

# Fail loudly if nothing was selected: clang-format without file arguments reads stdin, which is
# /dev/null in CI, so the check would pass having checked nothing at all.
if [[ ${#FILES[@]} -eq 0 ]]
then
    echo "error: no C++ files found. Check the file patterns in this script." >&2
    exit 1
fi

# Check the formatting if --check is given, otherwise format the files in place.
if [[ "${1:-}" == "--check" ]]
then
    # clang-format reports every violation and exits non-zero, so reaching the summary below
    # means that every file was already correctly formatted.
    clang-format --dry-run --Werror "${FILES[@]}"
    echo "Checked ${#FILES[@]} file(s), all correctly formatted."
else
    # Format one file at a time, so that the files actually changed can be reported. Always print
    # a summary, also when nothing needed changing.
    COUNT=0

    for FILE in "${FILES[@]}"
    do
        BEFORE="$(md5sum "$FILE")"
        clang-format -i "$FILE"
        AFTER="$(md5sum "$FILE")"

        if [[ "$BEFORE" != "$AFTER" ]]
        then
            echo "Formatted: $FILE"
            COUNT=$((COUNT + 1))
        fi
    done
    echo "Formatted $COUNT of ${#FILES[@]} file(s)."
fi
