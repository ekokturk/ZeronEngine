# Copyright (C) Eser Kokturk. All Rights Reserved.

set(GFX_OGL ZE_GRAPHICS_OPENGL=1)
if(WIN32)
    set(GFX_D3D ZE_GRAPHICS_D3D=1)
endif()

# ============ MODULE CREATE ============
zeron_add_module(ZeronGraphics "${CMAKE_CURRENT_SOURCE_DIR}")

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")

# OpenGL
if (DEFINED GFX_D3D)
    zeron_add_module_dependency(GLAD)
    if(NOT TARGET glad)
        message("Glad GL loader cannot be found!")
    endif()
    target_compile_definitions(ZeronGraphics PUBLIC ${GFX_D3D})
    find_package(OpenGL REQUIRED)
    list(APPEND MODULE_LIBS_PRIVATE glad ${OPENGL_gl_LIBRARY})
endif()

# Direct3D
if(DEFINED GFX_D3D)
    target_compile_definitions(ZeronGraphics PUBLIC ${GFX_D3D})
    list(APPEND MODULE_LIBS_PRIVATE dxgi d3d11 D3DCompiler)
endif()


# ============ MODULE INIT ============
target_include_directories(ZeronGraphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(ZeronGraphics
    PUBLIC
        ZeronCore
        ${MODULE_LIBS_PUBLIC}
    PRIVATE
        ${MODULE_LIBS_PRIVATE}
)