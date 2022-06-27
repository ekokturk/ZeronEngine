# Copyright (C) Eser Kokturk. All Rights Reserved.

cmake_minimum_required(VERSION 3.22.3)

# ============ PROJECT ============
project(ZERONENGINE)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

option(ZERON_SANDBOX        "Enable the Zeron Sandbox application" ON)
option(ZERON_TESTS          "Enable the Zeron tests" ON)
option(BUILD_SHARED_LIBS    "Build shared libraries" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")

# ============ COMPILER CONFIG =============
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# ============  PLATFORM CONFIG =============
if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE) # Symbols are exported by default for DLLs
elseif(UNIX AND NOT APPLE)
    set(LINUX TRUE)
elseif(ANDROID)
    set(MOBILE TRUE)
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

