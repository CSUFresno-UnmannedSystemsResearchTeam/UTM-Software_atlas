cmake_minimum_required(VERSION 3.20)
project(UTM_MissionPlanner 
    VERSION 0.1.0
    DESCRIPTION "Unmanned Traffic Management System and Mission Planner"
    LANGUAGES CXX C
)

# =============================================================================
# Project Standards
# =============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
# Project Options
# =============================================================================
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(ENABLE_VIDEO_RECORDING "Enable video recording support" ON)
option(ENABLE_THERMAL_SUPPORT "Enable thermal camera support" ON)

# =============================================================================
# Platform Detection and Configuration
# =============================================================================
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Detect target platform for cross-compilation support
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(TARGET_LINUX TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(TARGET_WINDOWS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(TARGET_MACOS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(TARGET_ANDROID TRUE)
endif()

# Architecture detection
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|ARM")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_ARM64 TRUE)
    else()
        set(TARGET_ARM32 TRUE)
    endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|AARCH64")
    set(TARGET_ARM64 TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(TARGET_X64 TRUE)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686|i386")
    set(TARGET_X86 TRUE)
endif()

# =============================================================================
# Compiler Settings
# =============================================================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic -Werror
        -Wno-unused-parameter
        -fconcepts -fcoroutines
        -pthread
        $<$<CONFIG:Debug>:-g3 -O0>
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
    )
elseif(MSVC)
    add_compile_options(
        /W4 /WX /permissive- /Zc:__cplusplus
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Release>:/O2>
    )
endif()

# =============================================================================
# Output Directories
# =============================================================================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# RPATH for Linux/macOS
if(UNIX AND NOT ANDROID)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:${CMAKE_INSTALL_PREFIX}/lib")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# =============================================================================
# Find Required Packages
# =============================================================================
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Qt6
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core Widgets Network Positioning Location 
    Multimedia MultimediaWidgets Sql Concurrent Test
)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Other required packages
find_package(SQLite3 REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs objdetect dnn videoio)
find_package(Boost 1.75 REQUIRED COMPONENTS system filesystem thread program_options)
find_package(Gnuradio REQUIRED COMPONENTS runtime blocks analog digital filter fft)
find_package(gnuradio-iio REQUIRED)
find_package(nlohmann_json 3.10 REQUIRED)
find_package(spdlog 1.9 REQUIRED)

# MAVLink
find_path(MAVLINK_INCLUDE_DIR
    NAMES mavlink/v2.0/common/mavlink.h
    HINTS ${CMAKE_SOURCE_DIR}/external/mavlink /usr/include /usr/local/include
)
if(NOT MAVLINK_INCLUDE_DIR)
    message(FATAL_ERROR "MAVLink not found. Install or add as submodule.")
endif()

# Video streaming libraries (Required for RTP/H.264 streaming)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale libavdevice)

# Optional additional video support
if(ENABLE_VIDEO_RECORDING)
    pkg_check_modules(GSTREAMER QUIET gstreamer-1.0 gstreamer-video-1.0)
endif()

# Testing
if(BUILD_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()
endif()

# =============================================================================
# Global Configuration
# =============================================================================
add_definitions(-DQT_NO_KEYWORDS)  # Avoid Qt conflicts with GNU Radio
if(TARGET_WINDOWS)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
endif()

# Common interface library
add_library(utm_common INTERFACE)
target_compile_features(utm_common INTERFACE cxx_std_20)
target_link_libraries(utm_common INTERFACE
    Threads::Threads
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)

# Global includes
include_directories(
    ${CMAKE_SOURCE_DIR}/libs
    ${MAVLINK_INCLUDE_DIR}
    ${GNURADIO_INCLUDE_DIRS}
)

# =============================================================================
# Helper Function for Creating Modules
# =============================================================================
function(create_utm_module MODULE_NAME MODULE_TYPE)
    # Parse arguments
    set(options HEADER_ONLY)
    set(oneValueArgs "")
    set(multiValueArgs SOURCES HEADERS PUBLIC_DEPS PRIVATE_DEPS DEFINITIONS UI_FILES)
    cmake_parse_arguments(MODULE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # Create library
    if(MODULE_HEADER_ONLY)
        add_library(${MODULE_NAME} INTERFACE)
        target_include_directories(${MODULE_NAME}
            INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
        )
        target_link_libraries(${MODULE_NAME} INTERFACE utm_common ${MODULE_PUBLIC_DEPS})
    else()
        add_library(${MODULE_NAME} ${MODULE_TYPE}
            ${MODULE_HEADERS}
            ${MODULE_SOURCES}
            ${MODULE_UI_FILES}
        )
        
        target_include_directories(${MODULE_NAME}
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src
                ${CMAKE_CURRENT_BINARY_DIR}  # For MOC/UIC files
        )
        
        target_link_libraries(${MODULE_NAME}
            PUBLIC utm_common ${MODULE_PUBLIC_DEPS}
            PRIVATE ${MODULE_PRIVATE_DEPS}
        )
        
        if(MODULE_DEFINITIONS)
            target_compile_definitions(${MODULE_NAME} PRIVATE ${MODULE_DEFINITIONS})
        endif()
        
        set_target_properties(${MODULE_NAME} PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON
        )
    endif()
    
    # Export for other modules
    set(${MODULE_NAME}_TARGET ${MODULE_NAME} CACHE INTERNAL "${MODULE_NAME} target")
    
    # Add tests if they exist
    if(BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        add_subdirectory(tests)
    endif()
    
    # Installation
    install(TARGETS ${MODULE_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )
    install(DIRECTORY include/
        DESTINATION include/utm/${MODULE_NAME}
        FILES_MATCHING PATTERN "*.h"
    )
endfunction()

# =============================================================================
# Add Subdirectories
# =============================================================================
add_subdirectory(libs)
add_subdirectory(apps)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# =============================================================================
# Status Output
# =============================================================================
message(STATUS "===============================================")
message(STATUS "Configuration:")
message(STATUS "  System:     ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler:   ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Qt6:        ${Qt6_VERSION}")
message(STATUS "  OpenCV:     ${OpenCV_VERSION}")
if(FFMPEG_FOUND)
    message(STATUS "  FFmpeg:     Found")
endif()
if(GSTREAMER_FOUND)
    message(STATUS "  GStreamer:  Found")
endif()
message(STATUS "===============================================")
