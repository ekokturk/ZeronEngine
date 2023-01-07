# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronRenderer ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(assimp)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")

list(APPEND MODULE_LIBS_PRIVATE stb-header-only assimp)
list(APPEND MODULE_LIBS_PUBLIC imgui)

# ============ MODULE INIT ============
target_include_directories(ZeronRenderer 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(ZeronRenderer
    PUBLIC
        ZeronCore
        ZeronGraphics
        ${MODULE_LIBS_PUBLIC}
    PRIVATE
        ${MODULE_LIBS_PRIVATE}
)

