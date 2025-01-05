# Append a new directory will be used by `zeron_compile_shaders` function
# `newShaderDir`: Shader directory path
macro(zeron_add_shader_directory newShaderDir)
    if(NOT EXISTS ${newShaderDir})
        message(FATAL_ERROR "${ZERON_ERROR_MSG} Shader directory '${newShaderDir} was not found'!")
    endif()
    get_property(_shaderDirs GLOBAL PROPERTY ZERON_SHADER_DIRECTORIES)
    set_property(GLOBAL PROPERTY ZERON_SHADER_DIRECTORIES ${_shaderDirs} ${newShaderDir})
endmacro()

# Compile all available HLSL shaders in the list of directories into the specified folder
# `target`: The target to compile shaders for
# `outputDir`: The relative directory in the build output folder compiled shaders will use 
function(zeron_compile_shaders target outputDir)
    get_property(_srcShaderDirs GLOBAL PROPERTY ZERON_SHADER_DIRECTORIES)

    if(NOT ZE_GLSLC_EXEC)
        message(FATAL_ERROR "${ZERON_ERROR_MSG} Vulkan SPIR-V compiler glslc was not found!")
    endif()

    if(WIN32)
        if(NOT ZE_FXC_EXEC)
            message(FATAL_ERROR "${ZERON_ERROR_MSG} Direct3D 11 compiler fxc was not found!")
        endif()
    endif()

    get_property(build_assets GLOBAL PROPERTY ZERON_BUILD_ASSETS_DIR)
    if(NOT EXISTS ${build_assets})
        set(build_assets "$<TARGET_FILE_DIR:${target}>")
    endif()

    # Find helper headers in the shader directories
    # We need to do this so they can be associated with source files
    foreach (_srcShaderDir IN LISTS _srcShaderDirs)
        file(GLOB_RECURSE _foundSrcHeaders "${_srcShaderDir}/*.h.hlsl")
        # Disable compilation of the utils files in Visual Studio
        set_source_files_properties(${_foundSrcHeaders} PROPERTIES VS_TOOL_OVERRIDE "None")
        list(APPEND _allSrcShaderHeaders ${_foundSrcHeaders})
        if(_foundSrcHeaders)
            list(APPEND _shaderIncludeDirs ${_srcShaderDir})
        endif()
    endforeach()

    set(_zslGenShaderDir "${CMAKE_BINARY_DIR}/generated/shaders")
    if(ANDROID AND EXISTS ${_zslGenShaderDir})
        # We clean generated shaders on project config
        file(REMOVE_RECURSE ${_zslGenShaderDir})
    endif()

    set(_zslOutDir "${build_assets}/${outputDir}")
    set(_allZslMetaData "")
    foreach (_srcShaderDir IN LISTS _srcShaderDirs)
        file(GLOB_RECURSE _zslShaders "${_srcShaderDir}/*.zsl")
        foreach (_zslShader IN LISTS _zslShaders)
            list(APPEND _allZslShaders ${_zslShader})
            cmake_path(ABSOLUTE_PATH _zslShader OUTPUT_VARIABLE _zslShaderAbs)
            get_filename_component(_zslShaderName ${_zslShader} NAME_WLE)
            set(_zslMetadata "${_zslGenShaderDir}/${_zslShaderName}.zsl.json")
            add_custom_command(
                OUTPUT ${_zslMetadata} 
                COMMAND ${CMAKE_COMMAND} -E echo "[ZERON] Compiling ${_zslShaderName}.zsl"
                COMMAND ${ZERON_PYTHON_EXE} -E zsl parse -shader "${_zslShaderAbs}" -outDir "${_zslGenShaderDir}"
                COMMAND ${CMAKE_COMMAND} -E env ZE_GLSLC_EXEC=${ZE_GLSLC_EXEC} ZE_FXC_EXEC=${ZE_FXC_EXEC}
                        ${ZERON_PYTHON_EXE} -E zsl compile -shaderName "${_zslShaderName}" -srcDir "${_zslGenShaderDir}" -outDir "${_zslOutDir}" 
                WORKING_DIRECTORY "${ZERON_TOOLS_DIR}/Python/packages/zsl"
                DEPENDS ${_zslShaderAbs}
                COMMAND_EXPAND_LISTS
                VERBATIM
            )
            list(APPEND _allZslMetaData ${_zslMetadata})
        endforeach ()
    endforeach ()

    # Create target to compile shaders
    add_custom_target(ShaderCompiler ALL DEPENDS ${_allZslMetaData})
    get_target_property(_targetFolder ${target} FOLDER)
    if(_targetFolder)
        set_target_properties(ShaderCompiler PROPERTIES FOLDER ${_targetFolder})
    endif()

    source_group("Shaders" FILES ${_allZslShaders})
    # source_group("Shaders" FILES ${_allSrcShaders} ${_allSrcShaderHeaders})
    # Visual Studio only detects HLSL Compiler for valid targetss
    # target_sources(${target} PUBLIC ${_allSrcShaders} ${_allSrcShaderHeaders})
    target_sources(${target} PUBLIC ${_allZslShaders})

    add_dependencies(${target} ShaderCompiler)
endfunction()
