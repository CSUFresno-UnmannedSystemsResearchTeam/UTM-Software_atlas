cmake_minimum_required(VERSION 3.25)
project(ATLAS_UTM VERSION 1.0.0 LANGUAGES CXX)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Quick)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(PostgreSQL REQUIRED)

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(libs)
add_subdirectory(apps)
add_subdirectory(tests)
