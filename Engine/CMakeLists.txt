# Copyright (C) Eser Kokturk. All Rights Reserved.

message("Generating Zeron Engine...")

SET(ZERON_CMAKE_UTILS "${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake" CACHE INTERNAL "Zeron CMake utilities")
include("${ZERON_CMAKE_UTILS}/ZeronMacros.cmake")

# ========== ZERON ENGINE =========
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DZE_DEBUG=1")

set(ZERON_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set(ZERON_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Modules")

add_subdirectory("${ZERON_SOURCE_DIR}/ZeronCore")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronGraphics")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronRenderer")

# Tests
if(ZERON_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tests")
endif()

# Resources
set(ZERON_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
add_subdirectory("${ZERON_RESOURCE_DIR}")
