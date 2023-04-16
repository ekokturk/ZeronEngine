# Copyright (C) Eser Kokturk. All Rights Reserved.

# Android

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")

if(ANDROID)
    find_library(ANDROID_DL_LIB NAMES libdl dl)
    find_library(ANDROID_LIBRARY_LIB android)
    find_library(ANDROID_LOG_LIB log)

    # Download pre-build files
    # TODO: Add 'https://android.googlesource.com/platform/frameworks/opt/gamesdk' as submodule for this
    set(GDK_VERSION "2023.1.0.0")
    set(GDK_LIB_FOLDER "GDK/${GDK_VERSION}")
    set(GDK_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${GDK_LIB_FOLDER})
    if(NOT EXISTS ${GDK_LIB_DIR})
        set(GDK_LINK https://dl.google.com/developers/android/agdk/agdk-libraries-${GDK_VERSION}.zip)
        file(DOWNLOAD 
            ${GDK_LINK}
            ${GDK_LIB_DIR}.zip 
            INACTIVITY_TIMEOUT 10
        )
        file(ARCHIVE_EXTRACT INPUT ${GDK_LIB_DIR}.zip  DESTINATION ${GDK_LIB_DIR})
        file(REMOVE ${GDK_LIB_DIR}.zip)
    endif()

    # Build Android GDK from source
    add_library(android-gdk STATIC 
        ${GDK_LIB_DIR}/include/common/gamesdk_common.h
        ${GDK_LIB_DIR}/include/game-activity/native_app_glue/android_native_app_glue.c
        ${GDK_LIB_DIR}/include/game-activity/native_app_glue/android_native_app_glue.h
        ${GDK_LIB_DIR}/include/game-activity/GameActivity.cpp
        ${GDK_LIB_DIR}/include/game-activity/GameActivity.h
        ${GDK_LIB_DIR}/include/game-activity/GameActivityEvents.cpp
        ${GDK_LIB_DIR}/include/game-activity/GameActivityEvents.h
        ${GDK_LIB_DIR}/include/game-activity/GameActivityLog.h
        ${GDK_LIB_DIR}/include/game-text-input/gamecommon.h
        ${GDK_LIB_DIR}/include/game-text-input/gametextinput.cpp
        ${GDK_LIB_DIR}/include/game-text-input/gametextinput.h

    )
    target_include_directories(android-gdk PUBLIC ${GDK_LIB_DIR}/include)
    target_link_libraries(android-gdk PUBLIC ${ANDROID_DL_LIB} ${ANDROID_LIBRARY_LIB} ${ANDROID_LOG_LIB})
endif()

