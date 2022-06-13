# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Source")

add_zeron_module(ZeronGraphics "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")

if (TARGET glad)
    target_compile_definitions(ZeronGraphics PUBLIC ZE_GRAPHICS_OPENGL=1)
    list(APPEND MODULE_LIBS_PRIVATE glad opengl32)
endif()

if(WIN32)
    # ======== D3D ========
    target_compile_definitions(ZeronGraphics PUBLIC ZE_GRAPHICS_D3D=1)
    list(APPEND MODULE_LIBS_PRIVATE dxgi)
    list(APPEND MODULE_LIBS_PRIVATE d3d11)
    list(APPEND MODULE_LIBS_PRIVATE D3DCompiler)
endif()


# ============ MODULE INIT ============
target_include_directories(ZeronGraphics PUBLIC "${SOURCE_DIRECTORY}")
target_link_libraries(ZeronGraphics
    PUBLIC
        ZeronCore
        ${MODULE_LIBS_PUBLIC}
    PRIVATE
        ${MODULE_LIBS_PRIVATE}
)