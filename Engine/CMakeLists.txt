# Copyright (C) Eser Kokturk. All Rights Reserved.

message("Generating Zeron Engine...")
# --- Config
include("${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake/ZeronMacros.cmake")

# ========== ENGINE DEPENDENCIES =========
set(ZERON_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set_property(GLOBAL PROPERTY ZERON_THIRDPARTY_DIR "${ZERON_THIRDPARTY_DIR}")

# ========== ZERON ENGINE =========
set(ZERON_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Modules")
init_output_directory("")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DZE_DEBUG=1")

add_subdirectory("${ZERON_SOURCE_DIR}/ZeronCore")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronGraphics")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronRenderer")

if(ZERON_TEST_FRAMEWORK)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tests")
endif()