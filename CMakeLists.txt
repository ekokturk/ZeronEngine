# Copyright (C) Eser Kokturk. All Rights Reserved.

cmake_minimum_required(VERSION 3.24.0)

# ============ PROJECT ============

project(ZERONENGINE VERSION 0.1.0)

option(ZERON_SANDBOX            "Enable the Zeron Sandbox application"          ON)
option(ZERON_TESTS              "Enable the Zeron tests"                        ON)
option(ZERON_UNIFY_OUTPUT_DIR   "Build binary outputs into the same directory"  OFF)
option(ZERON_PYTHON_USE_VENV    "Use Python virtual environment"                ON)

option(ZERON_ENABLE_SCRIPT      "Enable ZeronScript library"                    ON)

option(BUILD_SHARED_LIBS        "Build shared libraries"                        ON)

# ============ GENERAL CONFIG =============

set(ZERON_ENGINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Engine" CACHE INTERNAL "Zeron Engine Directory")
set(ZERON_TOOLS_DIR "${ZERON_ENGINE_DIR}/Tools" CACHE INTERNAL "Zeron Tools Directory")
set(ZERON_CMAKE_UTILS "${ZERON_TOOLS_DIR}/CMake" CACHE INTERNAL "Zeron CMake Utilities")
set(ZERON_ERROR_MSG "ZERON - [ERROR]:" CACHE BOOL "Prefix for engine related errors" FORCE)

message("ZERON - Configuring Project")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory" FORCE)
endif()

# Python
find_package(Python3 COMPONENTS Interpreter REQUIRED QUIET)
if(NOT DEFINED Python3_EXECUTABLE OR NOT EXISTS ${Python3_EXECUTABLE})
    message(FATAL_ERROR "${ZERON_ERROR_MSG} Python executable not found.")
endif()
message("ZERON -- Python: v${Python3_VERSION}")
if(ZERON_PYTHON_USE_VENV)
    set(SYSTEM_PYTHON_EXE_PATH ${Python3_EXECUTABLE})
    set(ZERON_PYTHON_VENV_DIR "${PROJECT_BINARY_DIR}/venv")
    message("ZERON --- Setting up virtual environment in ${ZERON_PYTHON_VENV_DIR}")
    set(CMAKE_MESSAGE_LOG_LEVEL ERROR)
    execute_process(COMMAND "${Python3_EXECUTABLE}" -m venv "${ZERON_PYTHON_VENV_DIR}" OUTPUT_QUIET COMMAND_ERROR_IS_FATAL ANY)
    set(ENV{VIRTUAL_ENV} "${ZERON_PYTHON_VENV_DIR}")
    set(Python3_FIND_VIRTUALENV FIRST)
    unset(Python3_EXECUTABLE)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    if(SYSTEM_PYTHON_EXE_PATH STREQUAL Python3_EXECUTABLE)
        message(FATAL_ERROR "${ZERON_ERROR_MSG} Python executable for venv is not found.")
    endif()
    execute_process(COMMAND 
        "${Python3_EXECUTABLE}" -m pip install -r "${ZERON_TOOLS_DIR}/Python/requirements.txt" 
        WORKING_DIRECTORY "${ZERON_TOOLS_DIR}/Python/"
        OUTPUT_QUIET ERROR_QUIET 
        COMMAND_ERROR_IS_FATAL ANY)
    unset(CMAKE_MESSAGE_LOG_LEVEL)
    message("ZERON --- Installed requirements")
endif()
set(ZERON_PYTHON_EXE ${Python3_EXECUTABLE})

# ============ COMPILER CONFIG =============

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MSVC_MIN_REQUIRED_VERSION 1914)
    if(MSVC_VERSION LESS ${MSVC_MIN_REQUIRED_VERSION})
        message(FATAL_ERROR "${ZERON_ERROR_MSG} MSVC compiler version ${MSVC_VERSION} (<${MSVC_MIN_REQUIRED_VERSION}) is not supported!")
    endif()
    message("ZERON -- Compiler: MSVC ${CMAKE_CXX_COMPILER_VERSION}")
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
    message("ZERON -- Compiler: Clang ${CMAKE_CXX_COMPILER_VERSION}")
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
    set(ZERON_PLATFORM_NAME "Windows")
    set(COMPILE_DEFS "ZE_PLATFORM_WIN32=1")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll" CACHE STRING "" FORCE)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE CACHE BOOL "Symbols are exported by default for DLLs" FORCE)
elseif(ANDROID)
    set(ZERON_PLATFORM_NAME "Android")
    set(COMPILE_DEFS "ZE_PLATFORM_ANDROID=1")
    set(MOBILE TRUE CACHE BOOL "" FORCE)
    set(UNIX OFF CACHE BOOL "" FORCE)
elseif(UNIX AND NOT APPLE)
    set(ZERON_PLATFORM_NAME "Linux")
    set(COMPILE_DEFS "ZE_PLATFORM_LINUX=1")
    set(LINUX ON CACHE BOOL "" FORCE)

    # In project shared libraries are not versioned
    set(CMAKE_PLATFORM_NO_VERSIONED_SONAME TRUE) 

    # Install path should set rpath to executable directory for depenedencies
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
else()
    message(FATAL_ERROR "${ZERON_ERROR_MSG} Platform is not supported!")
endif()
message("ZERON -- Platform: ${ZERON_PLATFORM_NAME}")

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

