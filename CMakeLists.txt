# Copyright (C) Eser Kokturk. All Rights Reserved.

cmake_minimum_required(VERSION 3.22.3)

# ============ PROJECT ============

project(ZERONENGINE VERSION 0.1.0)

option(ZERON_SANDBOX        "Enable the Zeron Sandbox application"          ON)
option(ZERON_TESTS          "Enable the Zeron tests"                        ON)
option(BUILD_SHARED_LIBS    "Build shared libraries"                        ON)
option(UNIFY_OUTPUT_DIR     "Build binary outputs into the same directory"  OFF)

# ============ GENERAL CONFIG =============

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# ============ COMPILER CONFIG =============
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MSVC_MIN_REQUIRED_VERSION 1914)
    if(MSVC_VERSION LESS ${MSVC_MIN_REQUIRED_VERSION})
        message(FATAL_ERROR "MSVC compiler version ${MSVC_VERSION} (<${MSVC_MIN_REQUIRED_VERSION}) is not supported!")
    endif()
    set(COMPILER_FLAGS 
        "/MP"                   # Enable parallel builds
        "/Zc:preprocessor"      # Enable new preprocessor
        "-DZE_COMPILER_MSVC=1"
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CLANG_MIN_REQUIRED_VERSION "14.0.0")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${CLANG_MIN_REQUIRED_VERSION})
        message(FATAL_ERROR "Clang compiler version ${CMAKE_CXX_COMPILER_VERSION} (<${CLANG_MIN_REQUIRED_VERSION}) is not supported!")
    endif()
    set(COMPILER_FLAGS "-DZE_COMPILER_CLANG=1")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-error=unused-but-set-variable")
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} is not supported!")
endif()
add_compile_options(
    ${COMPILER_FLAGS} 
    $<$<CONFIG:DEBUG>:-DZE_DEBUG=1>
)

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
else()
    message(FATAL_ERROR "Platform is not supported!")
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

