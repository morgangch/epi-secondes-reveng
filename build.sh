#!/bin/bash

SRC_DIR="src"
BUILD_DIR="build"
CXX="g++"
CXXFLAGS="-std=c++17 -O2"

CXXWIN32="i686-w64-mingw32-g++"
CXXWIN64="x86_64-w64-mingw32-g++"

OSXCROSS="/opt/osxcross/target/bin/o64-clang++"

mkdir -p "$BUILD_DIR"

if [ ! -d "$SRC_DIR" ]; then
    echo "Source directory '$SRC_DIR' does not exist."
    exit 1
fi

if ! command -v "$CXXWIN32" >/dev/null || ! command -v "$CXXWIN64" >/dev/null; then
    echo "Required cross-compilers for Windows are not installed."
    sudo apt-get install mingw-w64 -y
    if [ $? -ne 0 ]; then
        echo "Failed to install mingw-w64."
        exit 1
    fi
    CXXWIN32="i686-w64-mingw32-g++"
    CXXWIN64="x86_64-w64-mingw32-g++"
    echo "Cross-compilers installed successfully."
fi

# Compile for Windows 32-bit
$CXXWIN32 $CXXFLAGS -o "$BUILD_DIR/labyrinth_windows32.exe" $SRC_DIR/*.cpp -static -DWIN32

# Compile for Windows 64-bit
$CXXWIN64 $CXXFLAGS -o "$BUILD_DIR/labyrinth_windows64.exe" $SRC_DIR/*.cpp -static -DWIN64

# Compile for Linux
$CXX $CXXFLAGS -o "$BUILD_DIR/labyrinth_linux" $SRC_DIR/*.cpp -DLINUX

echo "Build complete. Binaries are in $BUILD_DIR/"