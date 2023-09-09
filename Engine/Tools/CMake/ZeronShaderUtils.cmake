# Append a new directory will be used by `zeron_compile_shaders` function
# `newShaderDir`: Shader directory path
macro(zeron_add_shader_directory newShaderDir)
    if(NOT EXISTS ${newShaderDir})
        message(FATAL_ERROR "Shader directory '${newShaderDir} was not found'!")
    endif()
    get_property(_shaderDirs GLOBAL PROPERTY ZERON_SHADER_DIRECTORIES)
    set_property(GLOBAL PROPERTY ZERON_SHADER_DIRECTORIES ${_shaderDirs} ${newShaderDir})
endmacro()

# Compile all available HLSL shaders in the list of directories into the specified folder
# `target`: The target to compile shaders for
# `outputDir`: The relative directory in the build output folder compiled shaders will use 
function(zeron_compile_shaders target outputDir)
    get_property(_srcShaderDirs GLOBAL PROPERTY ZERON_SHADER_DIRECTORIES)

    if(NOT GLSLC_EXEC)
        message(FATAL_ERROR "Vulkan SPIR-V compiler glslc was not found!")
    endif()

    get_property(build_assets GLOBAL PROPERTY ZERON_BUILD_ASSETS_DIR)
    if(NOT EXISTS ${build_assets})
        set(build_assets ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    set(_outShaderDir ${build_assets}/${outputDir})

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

    # Find shader source files in specified directories
    foreach (_srcShaderDir IN LISTS _srcShaderDirs)
        file(GLOB_RECURSE _srcShaderList 
            "${_srcShaderDir}/*.vert.hlsl"
            "${_srcShaderDir}/*.frag.hlsl"
            "${_srcShaderDir}/*.comp.hlsl"
        )
        foreach (_srcShader IN LISTS _srcShaderList)
            list(APPEND _allSrcShaders ${_srcShader})
            cmake_path(ABSOLUTE_PATH _srcShader OUTPUT_VARIABLE _srcShaderAbs)
            get_filename_component(_shaderName ${_srcShader} NAME_WLE)
            get_filename_component(_shaderExtension ${_shaderName} EXT)

            # ======================================================
            # ================= Compile for Vulkan =================
            set(_outShader "${_outShaderDir}/${_shaderName}.spv")
            set(_outShaderDep "${_outShaderDir}/${_shaderName}.d")
            add_custom_command(
                OUTPUT ${_outShader}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${_outShaderDir}
                COMMAND ${CMAKE_COMMAND} -E echo "[ZERON] Compiling SPIRV for '${target}': ${_srcShader} -> ${_outShader}"
                COMMAND ${GLSLC_EXEC}
                            -MD -MF "${_outShaderDep}" 
                            -O "${_srcShaderAbs}"
                            -o "${_outShader}" 
                            -I "${_shaderIncludeDirs}"
                            -DZE_SHADER_SPIRV
                            # --target-env=vulkan1.2 "${include_flags}"
                DEPENDS "${_srcShaderAbs}"
                BYPRODUCTS "${_outShaderDep}"
                DEPFILE "${_outShaderDep}"
                VERBATIM
                COMMAND_EXPAND_LISTS
            )

            add_custom_command(
                TARGET ${target} 
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_outShader} "$<TARGET_FILE_DIR:${target}>/${outputDir}/${_shaderName}.spv"
            )
            list(APPEND _allCompiledShaders ${_outShader})
            # =========================================================

            # ========================================================
            # ================= Compile for Direct3D =================
            if(WIN32)
                set(_shaderModelHLSL 5.0)
                if(_shaderExtension STREQUAL ".vert")
                    set(_shaderType Vertex)
                elseif(_shaderExtension STREQUAL ".frag")
                    set(_shaderType Pixel)
                elseif(_shaderExtension STREQUAL ".comp")
                    set(_shaderType Compute)
                else()
                    message(FATAL_ERROR "Invalid HLSL shader extension '${_shaderExtension}'!")
                endif()
                set_source_files_properties(${_srcShader} 
                    PROPERTIES 
                        VS_SHADER_FLAGS /I"${_shaderIncludeDirs}"
                        VS_SHADER_OBJECT_FILE_NAME $(OutDir)/${outputDir}/${_shaderName}.cso
                        VS_SHADER_TYPE ${_shaderType} 
                        VS_SHADER_MODEL ${_shaderModelHLSL}
                        VS_SHADER_ENTRYPOINT main
                        VS_SHADER_ENABLE_DEBUG 1
                )
            endif()
            # =========================================================
        endforeach ()
        
        list(LENGTH _srcShaderList _srcShaderListCount)
        cmake_path(RELATIVE_PATH _srcShaderDir BASE_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE _relativeSrcShaderDir)
        message("ZERON ---- Found (${_srcShaderListCount}) shaders to compile in '${_relativeSrcShaderDir}'")
    endforeach()

    # Create target to compile shaders
    add_custom_target(ShaderCompiler DEPENDS ${_allCompiledShaders})
    get_target_property(_targetFolder ${target} FOLDER)
    if(_targetFolder)
        set_target_properties(ShaderCompiler PROPERTIES FOLDER ${_targetFolder})
    endif()

    source_group("Shaders" FILES ${_allSrcShaders} ${_allSrcShaderHeaders})
    # Visual Studio only detects HLSL Compiler for valid targetss
    target_sources(${target} PUBLIC ${_allSrcShaders} ${_allSrcShaderHeaders})

    add_dependencies(${target} ShaderCompiler)
endfunction()
