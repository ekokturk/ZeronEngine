# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ THIRD PARTY ==============
add_zeron_module_dependency(GLFW)
add_zeron_module_dependency(GLAD)
add_zeron_module_dependency(SDL2)
add_zeron_module_dependency(stb)
add_zeron_module_dependency(ImGui)
add_zeron_module_dependency(assimp)

# ============ MODULE CREATE ============
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(SHADER_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Shaders")
set(RESOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Resources")

add_zeron_module(ZeronRenderer "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")

if (TARGET glad)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_GRAPHICS_OPENGL=1)
    list(APPEND MODULE_LIBS_PRIVATE glad opengl32)
endif()

if (TARGET glfw)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_GLFW=1)
    list(APPEND MODULE_LIBS_PRIVATE glfw)
endif()

if (TARGET SDL2)
    list(APPEND MODULE_LIBS_PRIVATE SDL2 SDL2main)
    target_compile_definitions(ZeronRenderer PUBLIC SDL_MAIN_HANDLED)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_SDL=1)
endif()

list(APPEND MODULE_LIBS_PRIVATE stb-header-only)
list(APPEND MODULE_LIBS_PUBLIC imgui)
list(APPEND MODULE_LIBS_PRIVATE assimp)

if(WIN32)
    target_compile_definitions(ZeronRenderer PUBLIC ZE_WINDOW_WIN32=1)

    # ======== D3D ========
    target_compile_definitions(ZeronRenderer PUBLIC ZE_GRAPHICS_D3D=1)
    list(APPEND MODULE_LIBS_PRIVATE dxgi)
    list(APPEND MODULE_LIBS_PRIVATE d3d11)
    list(APPEND MODULE_LIBS_PRIVATE D3DCompiler)

    # ======== HLSL ========
    file(GLOB_RECURSE HLSL_SHADERS "${SHADER_DIRECTORY}/HLSL/*.hlsl")
    source_group(TREE "${SHADER_DIRECTORY}/HLSL/" PREFIX "Shaders/HLSL" FILES ${HLSL_SHADERS})
    target_sources(ZeronRenderer PRIVATE ${HLSL_SHADERS})

    set(HLSL_SHADER_MODEL 5.0)
    foreach(HLSL_SHADER_PATH IN LISTS HLSL_SHADERS)
        get_filename_component(HLSL_SHADER_FILENAME ${HLSL_SHADER_PATH} NAME_WLE)
        get_filename_component(HLSL_SHADER_EXTENSTION ${HLSL_SHADER_FILENAME} EXT)
        if(HLSL_SHADER_EXTENSTION STREQUAL ".vs")
            set(HLSL_SHADER_TYPE Vertex)
        elseif(HLSL_SHADER_EXTENSTION STREQUAL ".fs")
            set(HLSL_SHADER_TYPE Pixel)
        else()
            message(FATAL_ERROR "Invalid HLSL shader extension!")
        endif()
        set_source_files_properties(${HLSL_SHADER_PATH} 
            PROPERTIES 
                VS_SHADER_OBJECT_FILE_NAME $(OutDir)/Resources/Shaders/${HLSL_SHADER_FILENAME}.cso
                VS_SHADER_TYPE ${HLSL_SHADER_TYPE} 
                VS_SHADER_MODEL ${HLSL_SHADER_MODEL}
                VS_SHADER_ENTRYPOINT main
        )
    endforeach()
    
endif()


# ============ MODULE INIT ============
target_include_directories(ZeronRenderer 
    PUBLIC 
        "${SOURCE_DIRECTORY}"
)

target_link_libraries(ZeronRenderer
    PUBLIC
        ZeronCore
        ${MODULE_LIBS_PUBLIC}
    PRIVATE
        ${MODULE_LIBS_PRIVATE}
)

add_custom_command(TARGET ZeronRenderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCE_DIRECTORY}" "$<TARGET_FILE_DIR:ZeronRenderer>/Resources/"
    COMMENT "Copying ZeronRenderer resources to output directory..."
)
