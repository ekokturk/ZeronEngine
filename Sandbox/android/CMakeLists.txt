# Copyright (C) Eser Kokturk. All Rights Reserved.

message("ZERON --- Platform: Android")

include("${ZERON_CMAKE_UTILS}/ZeronShaderCompiler.cmake")
include("${ZERON_CMAKE_UTILS}/ZeronUtils.cmake")

file(GLOB_RECURSE SANDBOX_ANDROID_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(SANDBOX_SOURCES ${SANDBOX_ANDROID_SOURCES} ${SANDBOX_SHARED_SOURCES})
source_group(TREE "${SANDBOX_DIR}" PREFIX "Source Files/" FILES ${SANDBOX_SOURCES})

add_library(Sandbox ${SANDBOX_SOURCES})
target_link_libraries(
    Sandbox 
        PUBLIC 
            ZeronPlatform ZeronRenderer ZeronGUI
)
target_include_directories(Sandbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SANDBOX_SHARED_DIR})
set_target_properties(Sandbox PROPERTIES DEBUG_POSTFIX "")

zeron_target_android_package(Sandbox "com.zeron.sandbox" Sandbox ${SANDBOX_DIR})
zeron_compile_shaders(Sandbox "Resources/Shaders")
zeron_copy_assets(Sandbox "Resources")