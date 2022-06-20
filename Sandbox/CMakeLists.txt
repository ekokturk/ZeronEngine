# Copyright (C) Eser Kokturk. All Rights Reserved.

# =========== VARIABLES ===========
init_output_directory("")

# =========== SETUP ===========
message("Generating Sandbox project...")

set(SANDBOX_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
file(GLOB_RECURSE SHARED_SOURCE "${SANDBOX_SOURCE_DIR}/shared/*.cpp" "${SANDBOX_SOURCE_DIR}/shared/*.h")
set(SANDBOX_SOURCE_FILES ${SHARED_SOURCE})
set(SANDBOX_LIBS ZeronCore)
set(SANDBOX_INCLUDES "${SANDBOX_SOURCE_DIR}/shared")

# =========== PLATFORM CONFIG ===========
if(WIN32)
    set(WIN32_DIR "${SANDBOX_SOURCE_DIR}/win32")
    file(GLOB_RECURSE WIN32_SOURCE "${WIN32_DIR}/*.cpp" "${WIN32_DIR}/*.h")
    list(APPEND SANDBOX_SOURCE_FILES ${WIN32_SOURCE})
    list(APPEND SANDBOX_INCLUDES ${WIN32_DIR})
    list(APPEND SANDBOX_LIBS ZeronRenderer)
endif()

if(UNIX AND NOT APPLE)
    set(LINUX_DIR "${SANDBOX_SOURCE_DIR}/linux")
    file(GLOB_RECURSE LINUX_SOURCE "${LINUX_DIR}/*.cpp" "${LINUX_DIR}/*.h")
    list(APPEND SANDBOX_SOURCE_FILES ${LINUX_SOURCE})
    list(APPEND SANDBOX_INCLUDES ${LINUX_DIR})
    list(APPEND SANDBOX_LIBS ZeronRenderer)
endif()

# =========== EXECUTABLE ===========
source_group(TREE "${SANDBOX_SOURCE_DIR}" PREFIX "Source Files/" FILES ${SANDBOX_SOURCE_FILES})
add_executable(Sandbox ${SANDBOX_SOURCE_FILES} )
target_include_directories(Sandbox PUBLIC ${SANDBOX_INCLUDES})
target_link_libraries(Sandbox PUBLIC ${SANDBOX_LIBS})

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