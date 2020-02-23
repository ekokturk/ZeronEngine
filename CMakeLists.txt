# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#CMake version
cmake_minimum_required(VERSION 3.14)

# ============ PROJECT ============
project(ZERONENGINE)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

include("${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake/Macros.cmake")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
# set(BUILD_SHARED_LIBS TRUE)

# ============  ENGINE =============
add_subdirectory(Engine)

# ============  SANDBOX =============
add_subdirectory(Sandbox)
# Set Sandbox as Visual Studio Startup Project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)


