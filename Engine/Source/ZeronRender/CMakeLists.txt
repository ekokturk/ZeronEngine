# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronRender ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(tinygltf)
zeron_add_module_dependency(stb)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")

list(APPEND MODULE_LIBS_PRIVATE stb tinygltf)
list(APPEND MODULE_LIBS_PUBLIC  )

# ============ MODULE INIT ============
target_include_directories(ZeronRender 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(ZeronRender
    PUBLIC
        ZeronCore
        ZeronGraphics
        ${MODULE_LIBS_PUBLIC}
    PRIVATE
        ${MODULE_LIBS_PRIVATE}
)

