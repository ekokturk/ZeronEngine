# Copyright (C) Eser Kokturk. All Rights Reserved.

# =========== SETUP ===========
message("ZERON - Generating: Sandbox Project")

set(SANDBOX_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SANDBOX_SHARED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shared")
file(GLOB_RECURSE SANDBOX_SHARED_SOURCES "${SANDBOX_SHARED_DIR}/*.cpp" "${SANDBOX_SHARED_DIR}/*.h")

if(UNIFY_OUTPUT_DIR)
    zeron_set_output_directory("")
else()
    zeron_set_output_directory("Sandbox")
endif()

if(WIN32)
    add_subdirectory(win32)
elseif(LINUX)
    add_subdirectory(linux)
elseif(ANDROID)
    add_subdirectory(android)
endif()

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
    set_target_properties(Sandbox PROPERTIES 
        VS_DPI_AWARE "PerMonitor" # enabled high dpi support
        VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Sandbox>
    )

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    cmake_path(GET CMAKE_BINARY_DIR PARENT_PATH BUILD_DIR)
    set(COMPILE_COMMANDS_FILE "${CMAKE_BINARY_DIR}/compile_commands.json")
    set(COMPILE_COMMANDS_SYMLINK "${BUILD_DIR}/compile_commands.json")

    if(NOT IS_SYMLINK ${COMPILE_COMMANDS_SYMLINK})
        file(REMOVE ${COMPILE_COMMANDS_SYMLINK})
    endif()

    execute_process(COMMAND cmake -E create_symlink
        ${COMPILE_COMMANDS_FILE}
        ${COMPILE_COMMANDS_SYMLINK}
    )
endif()
