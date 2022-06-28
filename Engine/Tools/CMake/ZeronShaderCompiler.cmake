
function(zeron_compile_hlsl target outputDir)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "Target does not exist!")
    endif()
    if(NOT WIN32)
        message(FATAL_ERROR "HLSL shaders are supported only in Windows builds!")
    endif()
    get_property(shader_dir GLOBAL PROPERTY ZERON_SHADERS_DIR)
    set(hlsl_shader_dir "${shader_dir}/HLSL")
    if(NOT EXISTS ${hlsl_shader_dir})
        message(FATAL_ERROR "Zeron HLSL shader directory does not exist!")
    endif()

    file(GLOB_RECURSE hlsl_shaders "${hlsl_shader_dir}/*.hlsl")
    list(LENGTH hlsl_shaders shader_count )
    if(shader_count EQUAL 0)
        message(WARNING "No HLSL shaders available to compile!")
        return()
    else()
        message("ZERON ---- Compiling Zeron HLSL shaders (Count: ${shader_count})")
    endif()

    source_group("Shaders/HLSL" FILES ${hlsl_shaders})
    target_sources(${target} PRIVATE ${hlsl_shaders})

    set(HLSL_SHADER_MODEL 5.0)
    foreach(HLSL_SHADER_PATH IN LISTS hlsl_shaders)
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
                VS_SHADER_OBJECT_FILE_NAME $(OutDir)/${outputDir}/${HLSL_SHADER_FILENAME}.cso
                VS_SHADER_TYPE ${HLSL_SHADER_TYPE} 
                VS_SHADER_MODEL ${HLSL_SHADER_MODEL}
                VS_SHADER_ENTRYPOINT main
                VS_SHADER_ENABLE_DEBUG 1
        )
    endforeach()
endfunction()
