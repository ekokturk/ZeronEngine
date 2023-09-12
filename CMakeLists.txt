# Copyright (C) Eser Kokturk. All Rights Reserved.

cmake_minimum_required(VERSION 3.24.0)

# ============ PROJECT ============

project(ZERONENGINE VERSION 0.1.0)

option(ZERON_SANDBOX            "Enable the Zeron Sandbox application"          ON)
option(ZERON_TESTS              "Enable the Zeron tests"                        ON)
option(ZERON_UNIFY_OUTPUT_DIR   "Build binary outputs into the same directory"  OFF)

option(BUILD_SHARED_LIBS        "Build shared libraries"                        ON)

# ============ GENERAL CONFIG =============

set(ZERON_ERROR_MSG "ZERON - [ERROR]:" CACHE BOOL "Prefix for engine related errors" FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# ============ COMPILER CONFIG =============

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MSVC_MIN_REQUIRED_VERSION 1914)
    if(MSVC_VERSION LESS ${MSVC_MIN_REQUIRED_VERSION})
        message(FATAL_ERROR "${ZERON_ERROR_MSG} MSVC compiler version ${MSVC_VERSION} (<${MSVC_MIN_REQUIRED_VERSION}) is not supported!")
    endif()
    set(COMPILER_FLAGS 
        "/MP"                   # Enable parallel builds
        "/Zc:preprocessor"      # Enable new preprocessor
        "-DZE_COMPILER_MSVC=1"
    )
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Supported build configurations" FORCE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CLANG_MIN_REQUIRED_VERSION "14.0.0")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${CLANG_MIN_REQUIRED_VERSION})
        message(FATAL_ERROR "${ZERON_ERROR_MSG} Clang compiler version ${CMAKE_CXX_COMPILER_VERSION} (<${CLANG_MIN_REQUIRED_VERSION}) is not supported!")
    endif()
    set(COMPILER_FLAGS "-DZE_COMPILER_CLANG=1")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile commands for code editors" FORCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-error=unused-but-set-variable" CACHE STRING "" FORCE)
else()
    message(FATAL_ERROR "${ZERON_ERROR_MSG} Compiler ${CMAKE_CXX_COMPILER_ID} is not supported!")
endif()

set(ZERON_COMPILE_OPTIONS ${COMPILER_FLAGS} $<$<CONFIG:DEBUG>:-DZE_DEBUG=1> CACHE STRING "Compiler options used for Zeron Engine" FORCE)
add_compile_options(${ZERON_COMPILE_OPTIONS})

# ============  PLATFORM CONFIG =============

if(WIN32)
    set(COMPILE_DEFS "ZE_PLATFORM_WIN32=1")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll" CACHE STRING "" FORCE)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE CACHE BOOL "Symbols are exported by default for DLLs" FORCE)
elseif(ANDROID)
    set(COMPILE_DEFS "ZE_PLATFORM_ANDROID=1")
    set(MOBILE TRUE CACHE BOOL "" FORCE)
    set(UNIX OFF CACHE BOOL "" FORCE)
elseif(UNIX AND NOT APPLE)
    set(COMPILE_DEFS "ZE_PLATFORM_LINUX=1")
    set(LINUX ON CACHE BOOL "" FORCE)
else()
    message(FATAL_ERROR "${ZERON_ERROR_MSG} Platform is not supported!")
endif()

if(WIN32 OR LINUX)
    set(DESKTOP TRUE CACHE BOOL "" FORCE)
endif()

set(ZERON_COMPILE_DEFINITIONS ${COMPILE_DEFS} CACHE STRING "Compiler definitions used for Zeron Engine" FORCE)
add_compile_definitions(${ZERON_COMPILE_DEFINITIONS} )

# ============  ENGINE =============

add_subdirectory(Engine)

# ============  SANDBOX =============

if(ZERON_SANDBOX)
    add_subdirectory(Sandbox)
endif()

