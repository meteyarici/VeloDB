#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

cd "$PROJECT_ROOT"

# Create build directory
BUILD_DIR="$PROJECT_ROOT/build/debug"
mkdir -p "$BUILD_DIR"

# Install Conan dependencies
echo "Installing Conan dependencies..."
conan install . --output-folder="$BUILD_DIR" --build=missing -s build_type=Debug

# Configure CMake
echo "Configuring CMake..."
cd "$BUILD_DIR"
cmake "$PROJECT_ROOT" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake

# Build
echo "Building..."
cmake --build . --config Debug

echo "Build complete! Executable: $BUILD_DIR/kvdb-server"

