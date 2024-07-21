# Copyright (C) Eser Kokturk. All Rights Reserved.

# V8 - Javascript Engine

include("${ZERON_CMAKE_UTILS}/ZeronAndroidUtils.cmake")

set(V8_VERSION "11.1")
if(WIN32)
    set(V8_PLATFORM "windows")
    set(V8_ARCH "x64")
    set(V8_EXT "dll")
else()
    set(V8_ARCH "x64")
    set(V8_PREFIX "lib")
    set(V8_ZLIB_PREFIX "libchrome_")
    if(LINUX)
        set(V8_PLATFORM "linux")
        set(V8_EXT "so")
    elseif(ANDROID)
        set(V8_PLATFORM "android")
        set(V8_EXT "cr.so")
        if(${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")
            set(V8_ARCH "arm64")
        endif()
    else()
        message(FATAL_ERROR "V8 Platform is not supported")
    endif()
endif()

set(V8_PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${V8_PLATFORM}/${V8_ARCH})
set(V8_VERSION_FILE ${V8_PLATFORM_DIR}/v8-version.txt)
set(V8_RELEASE_URL "https://github.com/ekokturk/v8-packager/releases/download")
set(V8_ASSET_URL "${V8_RELEASE_URL}/v8-${V8_VERSION}/${V8_PLATFORM}-${V8_ARCH}.zip")

set(V8_REQUIRES_DOWNLOAD FALSE)
if(NOT EXISTS ${V8_PLATFORM_DIR} OR NOT EXISTS ${V8_VERSION_FILE})
    set(V8_REQUIRES_DOWNLOAD TRUE)
else()
    file(READ ${V8_VERSION_FILE} V8_ASSET_VERSION)
    string(STRIP ${V8_ASSET_VERSION} V8_ASSET_VERSION)
    string(FIND "${V8_ASSET_VERSION}" "${V8_VERSION}." SUBSTRING_POSITION)
    if (NOT SUBSTRING_POSITION EQUAL 0)
        set(V8_REQUIRES_DOWNLOAD TRUE)
        file(REMOVE_RECURSE ${V8_PLATFORM_DIR})
    endif()
endif()

if(${V8_REQUIRES_DOWNLOAD})
    set(V8_DOWNLOADED_ASSET ${CMAKE_CURRENT_SOURCE_DIR}/v8.zip)
    file(DOWNLOAD ${V8_ASSET_URL} ${V8_DOWNLOADED_ASSET} STATUS V8_DOWNLOAD_STATUS)
    if(NOT V8_DOWNLOAD_STATUS EQUAL 0)
        message(FATAL_ERROR "Failed to download V8 library: ${V8_DOWNLOAD_STATUS}")
    endif()
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xf ${V8_DOWNLOADED_ASSET}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE V8_UNZIP_RESULT
        OUTPUT_QUIET
    )
    if(NOT V8_UNZIP_RESULT EQUAL 0)
        message(FATAL_ERROR "Error extracting V8 archive: ${V8_UNZIP_RESULT}")
    endif()
    file(REMOVE ${V8_DOWNLOADED_ASSET})
endif()

set(V8_INCLUDE_DIR  ${V8_PLATFORM_DIR}/include)
set(V8_LIB_DIR  ${V8_PLATFORM_DIR}/libs)
set(V8_RELEASE_DIR ${V8_LIB_DIR}/release)
set(V8_DEBUG_DIR  ${V8_LIB_DIR}/debug)
file(READ ${V8_PLATFORM_DIR}/definitions.txt V8_COMPILE_DEFS)

function(add_v8_lib_props target libName)
    set(_libDebugPath "${V8_DEBUG_DIR}/${libName}.${V8_EXT}")
    set(_libReleasePath "${V8_RELEASE_DIR}/${libName}.${V8_EXT}")

    if(WIN32)
        set_target_properties(${target} PROPERTIES 
            IMPORTED_IMPLIB_RELEASE "${_libReleasePath}.lib"
            IMPORTED_IMPLIB_DEBUG "${_libDebugPath}.lib"
        )
    elseif(ANDROID)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            zeron_android_add_external_lib(${_libDebugPath})
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            zeron_android_add_external_lib(${_libReleasePath})
        endif()
    endif()
    set_target_properties(${target} PROPERTIES IMPORTED_LOCATION_RELEASE ${_libReleasePath} IMPORTED_LOCATION_DEBUG ${_libDebugPath})

endfunction()

# Precompiled libraries
add_library(libv8_libbase SHARED IMPORTED)
add_library(libv8_platform SHARED IMPORTED)
add_library(libv8 SHARED IMPORTED)
add_library(libzlib SHARED IMPORTED)

add_v8_lib_props(libv8 "${V8_PREFIX}v8")
add_v8_lib_props(libv8_platform "${V8_PREFIX}v8_libplatform")
add_v8_lib_props(libv8_libbase "${V8_PREFIX}v8_libbase")
add_v8_lib_props(libzlib "${V8_ZLIB_PREFIX}zlib")

# Use precompiled libraries with an interface
add_library(v8_public INTERFACE)
target_link_libraries(v8_public INTERFACE libv8_platform libv8_libbase libzlib libv8)
target_compile_definitions(v8_public INTERFACE  ${V8_COMPILE_DEFS})

add_library(v8 INTERFACE)
target_link_libraries(v8 INTERFACE v8_public)
target_include_directories(v8 INTERFACE "${V8_INCLUDE_DIR}")
