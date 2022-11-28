# Copyright (C) Eser Kokturk. All Rights Reserved.

# SLD2 - Multi-platform Window Library

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS FALSE)
    set(EXTRA_LIBS vcruntime)
endif()

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")

option(SDL_TEST_ENABLED_BY_DEFAULT "" OFF)
# option(SDL2_DISABLE_UNINSTALL "" ON)

add_subdirectory(.submodule)

set(SDL2_FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/SDL2")
set_target_properties(SDL2main PROPERTIES FOLDER ${SDL2_FOLDER})
set_target_properties(SDL2 PROPERTIES FOLDER ${SDL2_FOLDER})
set_target_properties(uninstall PROPERTIES FOLDER ${SDL2_FOLDER})

set_property(GLOBAL PROPERTY SDL_JAVA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.submodule/android-project/app/src/main/java/)