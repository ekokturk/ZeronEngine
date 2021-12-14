# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# --- Config

# ========== ENGINE DEPENDENCIES =========
set(ZERON_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set_property(GLOBAL PROPERTY ZERON_THIRDPARTY_DIR "${ZERON_THIRDPARTY_DIR}")

# ========== ZERON ENGINE =========
set(ZERON_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
init_output_directory("")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DZE_DEBUG=1")

add_subdirectory("${ZERON_SOURCE_DIR}/ZeronCore")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronRenderer")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronClient")

option(BUILD_ZERON_TEST "Build Zeron Engine unit tests" ON)
if(BUILD_ZERON_TEST)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tests")
endif()