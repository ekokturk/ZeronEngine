# Copyright (C) Eser Kokturk. All Rights Reserved.

# =========== SETUP ===========
message("ZERON - Generating: Sandbox Project")

set(SANDBOX_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SANDBOX_SHARED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shared")
file(GLOB_RECURSE SANDBOX_SHARED_SOURCES "${SANDBOX_SHARED_DIR}/*.cpp" "${SANDBOX_SHARED_DIR}/*.h")

if(WIN32)
    add_subdirectory(win32)
elseif(LINUX)
    add_subdirectory(linux)
endif()

if(MSVC)
    set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Sandbox>)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    cmake_path(GET CMAKE_BINARY_DIR PARENT_PATH BUILD_DIR)
    add_custom_target(
        copy-compile-commands ALL
        ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/compile_commands.json" "${BUILD_DIR}/compile_commands.json"
    )
endif()
