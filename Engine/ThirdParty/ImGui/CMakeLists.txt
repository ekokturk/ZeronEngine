
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

if(TARGET glad)
    list(APPEND IMGUI_SOURCE_FILES 
        ${IMGUI_SOURCE_DIR}/imgui_impl_opengl3.h
        ${IMGUI_SOURCE_DIR}/imgui_impl_opengl3.cpp
        ${IMGUI_SOURCE_DIR}/imgui_impl_opengl3_loader.h
    )
    find_package(OpenGL REQUIRED)
    list(APPEND MODULE_LIBS_PRIVATE glad ${OPENGL_gl_LIBRARY})
endif()

if(WIN32)
    list(APPEND IMGUI_SOURCE_FILES 
        ${IMGUI_SOURCE_DIR}/imgui_impl_dx11.h
        ${IMGUI_SOURCE_DIR}/imgui_impl_dx11.cpp
    )
	list(APPEND IMGUI_LIBS dxgi d3d11)
endif()

add_library(imgui ${IMGUI_SOURCE_FILES})
set_target_properties(imgui PROPERTIES FOLDER ${ZERON_THIRDPARTY_FOLDER_PREFIX}/ImGui)

target_link_libraries(imgui 
    PRIVATE
        ${IMGUI_LIBS}
)
target_include_directories(imgui 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}	
    PRIVATE 
        ${IMGUI_SOURCE_DIR}		
)