# Copyright (C) Eser Kokturk. All Rights Reserved.

if(NOT ZERON_ENABLE_SCRIPT)
    return()
endif()

# ============ MODULE CREATE ============
zeron_add_module(ZeronScript ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(v8)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE ZeronCore v8)
set(MODULE_LIBS_PUBLIC v8_public)
set(MODULE_INCLUDES "")

# ============ MODULE INIT ============
target_include_directories(ZeronScript 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Private/"
)

target_link_libraries(ZeronScript
    PUBLIC
        ${MODULE_LIBS_PUBLIC}
    PRIVATE
        ${MODULE_LIBS_PRIVATE}
)

target_compile_definitions(ZeronScript PUBLIC ZE_SCRIPT=1)
