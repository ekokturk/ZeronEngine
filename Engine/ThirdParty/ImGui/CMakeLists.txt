
# Copyright (C) Eser Kokturk. All Rights Reserved.

# ImGui - Graphical User Interface Library

set(SUBMODULE_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/.submodule)
set(IMGUI_SOURCE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

file(REMOVE_RECURSE ${IMGUI_SOURCE_DIR})

file(GLOB IMGUI_SOURCE_MAIN RELATIVE ${SUBMODULE_DIR} ${SUBMODULE_DIR}/*.h ${SUBMODULE_DIR}/*.cpp)
foreach(filename ${IMGUI_SOURCE_MAIN})
    configure_file(${SUBMODULE_DIR}/${filename} ${IMGUI_SOURCE_DIR}/${filename} COPYONLY)
endforeach()
file(COPY ${SUBMODULE_DIR}/backends/ DESTINATION ${IMGUI_SOURCE_DIR}/)

set(IMGUI_SOURCE_FILES 
    ${IMGUI_SOURCE_DIR}/imgui.h
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imconfig.h
    ${IMGUI_SOURCE_DIR}/imgui_internal.h
    ${IMGUI_SOURCE_DIR}/imstb_rectpack.h
    ${IMGUI_SOURCE_DIR}/imstb_textedit.h
    ${IMGUI_SOURCE_DIR}/imstb_truetype.h
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
)

include("${ZERON_CMAKE_UTILS}/ZeronShaderUtils.cmake")
zeron_add_shader_directory("${CMAKE_CURRENT_SOURCE_DIR}/utils")

add_library(imgui ${IMGUI_SOURCE_FILES})
set_target_properties(imgui PROPERTIES FOLDER ${ZERON_THIRDPARTY_FOLDER_PREFIX}/ImGui)

target_include_directories(imgui 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}	
    PRIVATE 
        ${IMGUI_SOURCE_DIR}		
)