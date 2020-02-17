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

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

# ============  ENGINE =============
add_subdirectory(Engine)


