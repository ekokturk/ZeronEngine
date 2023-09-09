# Copyright (C) Eser Kokturk. All Rights Reserved.

# =========== SETUP ===========
message("ZERON - Generating: Sandbox Project")

include("${ZERON_CMAKE_UTILS}/ZeronProjectUtils.cmake")
include("${ZERON_CMAKE_UTILS}/ZeronShaderUtils.cmake")
include("${ZERON_CMAKE_UTILS}/ZeronAndroidUtils.cmake")

set(SANDBOX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
file(GLOB_RECURSE SANDBOX_SOURCES "${SANDBOX_DIR}/*.cpp" "${SANDBOX_DIR}/*.h")
source_group(TREE "${SANDBOX_DIR}" PREFIX "Source Files/" FILES ${SANDBOX_SOURCES})

zeron_create_project(Sandbox "${SANDBOX_SOURCES}")

target_include_directories(Sandbox PUBLIC ${SANDBOX_DIR})
target_link_libraries(Sandbox 
    PUBLIC ZeronPlatform ZeronRender ZeronGUI
)
set_target_properties(Sandbox PROPERTIES FOLDER "Sandbox")

# Platfom specific setup
set(ZERON_ANDROID_CONFIG_SCREEN_ORIENTATION "landscape")
zeron_android_configure_package(Sandbox "com.zeron.sandbox" Sandbox ${SANDBOX_DIR})

# Asset setup
zeron_compile_shaders(Sandbox "Resources/Shaders")
zeron_copy_assets(Sandbox "Resources")


