#!/bin/bash

# =============================================================================
# UTM Mission Planner Cross-Platform Build Script
# =============================================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Default values
BUILD_TYPE="Debug"
BUILD_DIR="build"
TARGET_PLATFORM=""
TARGET_ARCH=""
TOOLCHAIN_FILE=""
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
CLEAN_BUILD=false
RUN_TESTS=false
VERBOSE=false

print_info() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }

usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Cross-platform build script for UTM Mission Planner

OPTIONS:
    -h, --help              Show this help message
    -t, --type TYPE         Build type (Debug|Release|RelWithDebInfo) [Default: Debug]
    -d, --dir DIR           Build directory [Default: build]
    -p, --platform PLATFORM Target platform (linux|windows|macos|android|raspberry-pi)
    -a, --arch ARCH         Target architecture (x64|x86|arm64|armhf)
    --toolchain FILE        CMake toolchain file for cross-compilation
    -j, --jobs N            Number of parallel jobs [Default: ${JOBS}]
    -c, --clean             Clean build
    -T, --test              Run tests after building
    -v, --verbose           Verbose build output
    
EXAMPLES:
    # Native build (auto-detect platform)
    $0
    
    # Cross-compile for Windows from Linux
    $0 --platform windows --arch x64
    
    # Cross-compile for Raspberry Pi
    $0 --platform raspberry-pi --arch armhf
    
    # Android build
    $0 --platform android --arch arm64 --toolchain \$ANDROID_NDK/build/cmake/android.toolchain.cmake
    
EOF
    exit 0
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help) usage ;;
        -t|--type) BUILD_TYPE="$2"; shift 2 ;;
        -d|--dir) BUILD_DIR="$2"; shift 2 ;;
        -p|--platform) TARGET_PLATFORM="$2"; shift 2 ;;
        -a|--arch) TARGET_ARCH="$2"; shift 2 ;;
        --toolchain) TOOLCHAIN_FILE="$2"; shift 2 ;;
        -j|--jobs) JOBS="$2"; shift 2 ;;
        -c|--clean) CLEAN_BUILD=true; shift ;;
        -T|--test) RUN_TESTS=true; shift ;;
        -v|--verbose) VERBOSE=true; shift ;;
        *) print_error "Unknown option: $1"; usage ;;
    esac
done

# Auto-detect platform if not specified
if [ -z "$TARGET_PLATFORM" ]; then
    case "$(uname -s)" in
        Linux*) TARGET_PLATFORM="linux" ;;
        Darwin*) TARGET_PLATFORM="macos" ;;
        CYGWIN*|MINGW*|MSYS*) TARGET_PLATFORM="windows" ;;
        *) print_error "Unknown platform"; exit 1 ;;
    esac
fi

# Auto-detect architecture if not specified
if [ -z "$TARGET_ARCH" ]; then
    case "$(uname -m)" in
        x86_64|amd64) TARGET_ARCH="x64" ;;
        i686|i386) TARGET_ARCH="x86" ;;
        arm64|aarch64) TARGET_ARCH="arm64" ;;
        armv7*|armhf) TARGET_ARCH="armhf" ;;
        *) TARGET_ARCH="x64" ;;
    esac
fi

# Set build directory based on target
BUILD_DIR="${BUILD_DIR}-${TARGET_PLATFORM}-${TARGET_ARCH}"

print_info "Build Configuration:"
print_info "  Host: $(uname -s) $(uname -m)"
print_info "  Target: ${TARGET_PLATFORM} ${TARGET_ARCH}"
print_info "  Build Type: ${BUILD_TYPE}"
print_info "  Build Dir: ${BUILD_DIR}"
print_info "  Jobs: ${JOBS}"
echo ""

# Clean if requested
if [ "$CLEAN_BUILD" = true ]; then
    print_info "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake based on platform
CMAKE_ARGS=(
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    -DBUILD_TESTS="$RUN_TESTS"
)

# Platform-specific configuration
case "$TARGET_PLATFORM" in
    windows)
        if [ "$(uname -s)" = "Linux" ]; then
            # Cross-compile from Linux to Windows
            CMAKE_ARGS+=(
                -DCMAKE_SYSTEM_NAME=Windows
                -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc
                -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++
                -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres
                -DCMAKE_FIND_ROOT_PATH=/usr/x86_64-w64-mingw32
                -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
                -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
                -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
            )
        fi
        ;;
    
    raspberry-pi)
        # Cross-compile for Raspberry Pi
        if [ "$TARGET_ARCH" = "armhf" ]; then
            CMAKE_ARGS+=(
                -DCMAKE_SYSTEM_NAME=Linux
                -DCMAKE_SYSTEM_PROCESSOR=arm
                -DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc
                -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++
                -DCMAKE_FIND_ROOT_PATH=/usr/arm-linux-gnueabihf
                -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
                -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
                -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
            )
        elif [ "$TARGET_ARCH" = "arm64" ]; then
            CMAKE_ARGS+=(
                -DCMAKE_SYSTEM_NAME=Linux
                -DCMAKE_SYSTEM_PROCESSOR=aarch64
                -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc
                -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++
            )
        fi
        ;;
    
    android)
        if [ -z "$TOOLCHAIN_FILE" ]; then
            print_error "Android build requires --toolchain pointing to Android NDK toolchain"
            exit 1
        fi
        CMAKE_ARGS+=(
            -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE"
            -DANDROID_ABI="${TARGET_ARCH}"
            -DANDROID_PLATFORM=android-28
        )
        ;;
esac

# Add custom toolchain if specified
if [ -n "$TOOLCHAIN_FILE" ] && [ "$TARGET_PLATFORM" != "android" ]; then
    CMAKE_ARGS+=(-DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE")
fi

# Verbose output
if [ "$VERBOSE" = true ]; then
    CMAKE_ARGS+=(-DCMAKE_VERBOSE_MAKEFILE=ON)
fi

# Configure
print_info "Configuring with CMake..."
if ! cmake .. "${CMAKE_ARGS[@]}"; then
    print_error "CMake configuration failed!"
    exit 1
fi

# Build
print_info "Building with $JOBS parallel jobs..."
if ! cmake --build . -j"$JOBS"; then
    print_error "Build failed!"
    exit 1
fi

print_success "Build complete!"

# Run tests if requested (only for native builds)
if [ "$RUN_TESTS" = true ] && [ -z "$TOOLCHAIN_FILE" ]; then
    print_info "Running tests..."
    if ! ctest --output-on-failure; then
        print_warning "Some tests failed!"
    else
        print_success "All tests passed!"
    fi
fi

# Print output location
EXECUTABLE="bin/utm_mission_planner"
[ "$TARGET_PLATFORM" = "windows" ] && EXECUTABLE="${EXECUTABLE}.exe"

if [ -f "$EXECUTABLE" ]; then
    print_success "Executable: $(pwd)/$EXECUTABLE"
else
    print_warning "Executable not found at expected location"
fi

cd ..
print_success "Build complete for ${TARGET_PLATFORM}-${TARGET_ARCH}!"
