# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
add_zeron_module(ZeronRenderer ${CMAKE_CURRENT_SOURCE_DIR})

# ============ DEPENDENCIES ============
set(MODULE_LIBRARIES "")
set(MODULE_INCLUDES "")

if (TARGET glad)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_GRAPHICS_OPENGL=1)
    list(APPEND MODULE_LIBRARIES glad opengl32)
endif()

if (TARGET glfw)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_GLFW=1)
    list(APPEND MODULE_LIBRARIES glfw)
endif()

if (TARGET SDL2)
    list(APPEND MODULE_LIBRARIES SDL2 SDL2main)
    target_compile_definitions(ZeronRenderer PUBLIC SDL_MAIN_HANDLED)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_SDL=1)
endif()

if(WIN32)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_WIN32=1)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_GRAPHICS_D3D11=1)
    list(APPEND MODULE_LIBRARIES d3d11)
endif()


# ============ MODULE INIT ============
target_include_directories(ZeronRenderer 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(ZeronRenderer
    PRIVATE
        ZeronCore
        ${MODULE_LIBRARIES}
)
