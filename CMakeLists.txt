# Copyright (C) Eser Kokturk. All Rights Reserved.

cmake_minimum_required(VERSION 3.22.3)

# ============ PROJECT ============

project(ZERONENGINE VERSION 0.1.0)

option(ZERON_SANDBOX        "Enable the Zeron Sandbox application"          ON)
option(ZERON_TESTS          "Enable the Zeron tests"                        ON)
option(BUILD_SHARED_LIBS    "Build shared libraries"                        ON)
option(UNIFY_OUTPUT_DIR     "Build binary outputs into the same directory"  OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# ============ COMPILER CONFIG =============
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/MP") # Enable parallel builds
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# ============  PLATFORM CONFIG =============
if(WIN32)
    add_compile_definitions(ZE_PLATFORM_WIN32=1)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE) # Symbols are exported by default for DLLs
elseif(ANDROID)
    add_compile_definitions(ZE_PLATFORM_ANDROID=1)
    set(MOBILE TRUE)
    set(UNIX OFF)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(ZE_PLATFORM_LINUX=1)
    set(LINUX ON)
endif()

if(WIN32 OR LINUX)
    set(DESKTOP TRUE)
endif()

# ============  ENGINE =============
add_subdirectory(Engine)

# ============  SANDBOX =============
if(ZERON_SANDBOX)
    add_subdirectory(Sandbox)
endif()

