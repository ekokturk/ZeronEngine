# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#CMake version
cmake_minimum_required(VERSION 3.14)

include("${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake/Macros.cmake")

# ============ PROJECT ============
project(ZERONENGINE)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS TRUE)

# ============  PLATFORM CONFIG =============
# WINDOWS
if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE) # Symbols are exported by default for DLLs
endif()

# ============  ENGINE CONFIG =============
option(BUILD_SANDBOX "Generate Sandbox" ON)

# ============  ENGINE =============
message("Generating Zeron Engine...")
add_subdirectory(Engine)

# ============  SANDBOX =============
if(BUILD_SANDBOX)
    message("Generating Sandbox project...")
    add_subdirectory(Sandbox)
    # Set Sandbox as Visual Studio Startup Project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
endif()

