# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronPlatform ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(stb)
if(DESKTOP)
    zeron_add_module_dependency(GLFW)
elseif(ANDROID)
    zeron_add_module_dependency(Android)
endif()

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE stb)
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")

if (ANDROID)
    if(NOT TARGET android-gdk)
        message(FATAL_ERROR "${ZERON_ERROR_MSG} Unable to locate Android GDK")
    endif()
    # Needed for: https://developer.android.com/games/agdk/game-activity/migrate-native-activity#issue
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u \ Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")
    list(APPEND MODULE_LIBS_PUBLIC android-gdk)
endif()

if (TARGET glfw)
    list(APPEND MODULE_LIBS_PRIVATE glfw)
    target_compile_definitions(ZeronPlatform PUBLIC ZE_WINDOW_GLFW=1)
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
		${MODULE_LIBS_PUBLIC}
	PRIVATE
		${MODULE_LIBS_PRIVATE}
)