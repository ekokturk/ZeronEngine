# Copyright (C) Eser Kokturk. All Rights Reserved.

cmake_minimum_required(VERSION 3.22.3)

# ============ PROJECT ============
project(ZERONENGINE)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")
set(BUILD_SHARED_LIBS TRUE)

# ============  PLATFORM CONFIG =============
# WINDOWS
if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE) # Symbols are exported by default for DLLs
endif()

# ============  ENGINE =============
add_subdirectory(Engine)

# ============  SANDBOX =============
if(ZERON_SANDBOX)
    add_subdirectory(Sandbox)
endif()

