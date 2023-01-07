# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronPlatform ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
if(DESKTOP)
    zeron_add_module_dependency(GLFW)
endif()
zeron_add_module_dependency(SDL2)
zeron_add_module_dependency(stb)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")

if (TARGET glfw)
    list(APPEND MODULE_LIBS_PRIVATE glfw)
    target_compile_definitions(ZeronPlatform PUBLIC ZE_WINDOW_GLFW=1)
endif()

if (TARGET SDL2)
    list(APPEND MODULE_LIBS_PRIVATE SDL2 SDL2main)
    target_compile_definitions(ZeronPlatform PUBLIC ZE_WINDOW_SDL=1)
    if(NOT ANDROID)
        target_compile_definitions(ZeronPlatform PUBLIC SDL_MAIN_HANDLED)
    endif()
endif()

if(WIN32)
    target_compile_definitions(ZeronPlatform PUBLIC ZE_WINDOW_WIN32=1)
endif()

# ============ MODULE INIT ============
target_include_directories(ZeronPlatform 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ZeronPlatform
    PUBLIC
		ZeronCore
	PRIVATE
		${MODULE_LIBS_PRIVATE}
)