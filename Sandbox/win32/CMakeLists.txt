# Copyright (C) Eser Kokturk. All Rights Reserved.

message("ZERON --- Platform: Win32")

include("${ZERON_CMAKE_UTILS}/ZeronShaderCompiler.cmake")
include("${ZERON_CMAKE_UTILS}/ZeronUtils.cmake")

file(GLOB_RECURSE SANDBOX_WIN32_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(SANDBOX_SOURCES ${SANDBOX_WIN32_SOURCES} ${SANDBOX_SHARED_SOURCES})
source_group(TREE "${SANDBOX_DIR}" PREFIX "Source Files/" FILES ${SANDBOX_SOURCES})

add_executable(Sandbox ${SANDBOX_SOURCES})
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SANDBOX_SHARED_DIR})
target_link_libraries(Sandbox PUBLIC ZeronRenderer)
set_target_properties(Sandbox PROPERTIES FOLDER "Sandbox")

zeron_copy_runtime_dependencies(Sandbox)
zeron_compile_shaders(Sandbox "Resources/Shaders")
zeron_copy_assets(Sandbox "Resources")
