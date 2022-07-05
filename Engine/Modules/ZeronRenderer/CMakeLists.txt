# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronRenderer ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
if(DESKTOP)
    zeron_add_module_dependency(GLFW)
endif()
zeron_add_module_dependency(SDL2)
zeron_add_module_dependency(stb)
zeron_add_module_dependency(ImGui)
zeron_add_module_dependency(assimp)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")

if (TARGET glfw)
    list(APPEND MODULE_LIBS_PRIVATE glfw)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_GLFW=1)
endif()

if (TARGET SDL2)
    list(APPEND MODULE_LIBS_PRIVATE SDL2 SDL2main)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_SDL=1)
    if(NOT ANDROID)
        target_compile_definitions(ZeronRenderer PUBLIC SDL_MAIN_HANDLED)
    endif()
endif()

list(APPEND MODULE_LIBS_PRIVATE stb-header-only assimp)
list(APPEND MODULE_LIBS_PUBLIC imgui)

if(WIN32)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_WIN32=1)
endif()


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

