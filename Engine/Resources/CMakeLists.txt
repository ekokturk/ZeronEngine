# Copyright (C) Eser Kokturk. All Rights Reserved.

set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
set(PLATFORM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Platform")

set_property(GLOBAL PROPERTY ZERON_ASSETS_DIR   ${ASSETS_DIR})
set_property(GLOBAL PROPERTY ZERON_FONTS_DIR    ${ASSETS_DIR}/Fonts)
set_property(GLOBAL PROPERTY ZERON_MODELS_DIR   ${ASSETS_DIR}/Models)
set_property(GLOBAL PROPERTY ZERON_TEXTURES_DIR ${ASSETS_DIR}/Textures)

zeron_add_shader_directory("${CMAKE_CURRENT_SOURCE_DIR}/Shaders")

if(ANDROID)
    set_property(GLOBAL PROPERTY ZERON_ANDROID_PROJECT_DIR ${PLATFORM_DIR}/Android/project)
endif()