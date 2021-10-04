# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# --- Config

# ========== ENGINE DEPENDENCIES =========
set(ZERON_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set_property(GLOBAL PROPERTY ZERON_THIRDPARTY_DIR "${ZERON_THIRDPARTY_DIR}")
add_subdirectory(${ZERON_THIRDPARTY_DIR})

# ========== ZERON ENGINE =========
set(ZERON_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
init_output_directory("")

add_subdirectory("${ZERON_SOURCE_DIR}/ZeronCore")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronRenderer")
add_subdirectory("${ZERON_SOURCE_DIR}/ZeronClient")
