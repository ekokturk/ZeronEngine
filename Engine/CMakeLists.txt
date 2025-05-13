# Copyright (C) Eser Kokturk. All Rights Reserved.

message("ZERON - Generating: Zeron Engine")

include("${ZERON_CMAKE_UTILS}/Internal/ZeronEngineHelpers.cmake")
include("${ZERON_CMAKE_UTILS}/ZeronProjectUtils.cmake")

if(ZERON_UNIFY_OUTPUT_DIR)
    zeron_set_output_directory("")
endif()

# ========== ZERON ENGINE =========
set(ZERON_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set(ZERON_THIRDPARTY_FOLDER_PREFIX "ZeronEngine/ThirdParty")

set(ZERON_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

add_subdirectory("${ZERON_SOURCE_DIR}/ZeronCore")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronDB")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronPlatform")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronGraphics")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronNet")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronPhysics")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronRender")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronScript")


add_subdirectory("${ZERON_SOURCE_DIR}/ZeronGUI")

# Tests
if(ZERON_TESTS)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tests")
endif()

# Resources
set(ZERON_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
add_subdirectory("${ZERON_RESOURCE_DIR}")
