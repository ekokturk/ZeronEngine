# Copyright (C) Eser Kokturk. All Rights Reserved.

if(WIN32)
    set(GFX_D3D ZE_GRAPHICS_D3D=1)
endif()
set(GFX_VULKAN ZE_GRAPHICS_VULKAN=1)

# ============ MODULE CREATE ============
zeron_add_module(ZeronGraphics "${CMAKE_CURRENT_SOURCE_DIR}")

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_INCLUDES_PRIVATE "")

# Direct3D
if(DEFINED GFX_D3D)
    message("ZERON ---- Graphics: Direct3D 11")
    target_compile_definitions(ZeronGraphics PUBLIC ${GFX_D3D})
    list(APPEND MODULE_LIBS_PRIVATE dxgi d3d11 D3DCompiler)
endif()

# Vulkan
if(DEFINED GFX_VULKAN)
    message("ZERON ---- Graphics: Vulkan")
    zeron_add_module_dependency(Vulkan-Headers)
    zeron_add_module_dependency(glslc)
    target_compile_definitions(ZeronGraphics PUBLIC ${GFX_VULKAN})
    list(APPEND MODULE_LIBS_PUBLIC Vulkan::Headers)
endif()

# ============ MODULE INIT ============
target_include_directories(ZeronGraphics 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${MODULE_INCLUDES_PRIVATE}
    PRIVATE
)
target_link_libraries(ZeronGraphics
    PUBLIC
        ZeronCore
        ${MODULE_LIBS_PUBLIC}
    PRIVATE
        ${MODULE_LIBS_PRIVATE}
)