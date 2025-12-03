#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Default to release build
BUILD_TYPE="${1:-release}"
BUILD_DIR="$PROJECT_ROOT/build/$BUILD_TYPE"

if [ ! -f "$BUILD_DIR/kvdb-server" ]; then
    echo "Error: kvdb-server not found at $BUILD_DIR/kvdb-server"
    echo "Please build the project first using:"
    echo "  ./scripts/build_${BUILD_TYPE}.sh"
    exit 1
fi

echo "Running kvdb-server from $BUILD_DIR..."
"$BUILD_DIR/kvdb-server"

