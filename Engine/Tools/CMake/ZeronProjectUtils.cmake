# Create a new target that will be used as Zeron Engine project
# `target`: Target name
# `targetSources`: List of C++ source files for the project
macro(zeron_create_project target targetSources)
    # Pre-configuration
    if(ZERON_UNIFY_OUTPUT_DIR)
        zeron_set_output_directory("")
    endif()

    # Create target
    if(ANDROID)
        # Cannot create an executable for Android
        add_library(${target} ${targetSources})
        set_target_properties(${target} PROPERTIES DEBUG_POSTFIX "")
    else()
        add_executable(${target} ${targetSources})
    endif()

    # Compiler configuration
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${target})
        set_target_properties(${target} PROPERTIES 
            VS_DPI_AWARE "PerMonitor" # enabled high dpi support
            VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${target}>
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        cmake_path(GET CMAKE_BINARY_DIR PARENT_PATH BUILD_DIR)
        set(COMPILE_COMMANDS_FILE "${CMAKE_BINARY_DIR}/compile_commands.json")
        set(COMPILE_COMMANDS_SYMLINK "${BUILD_DIR}/compile_commands.json")

        if(NOT IS_SYMLINK ${COMPILE_COMMANDS_SYMLINK})
            file(REMOVE ${COMPILE_COMMANDS_SYMLINK})
        endif()

        execute_process(COMMAND cmake -E create_symlink
            ${COMPILE_COMMANDS_FILE}
            ${COMPILE_COMMANDS_SYMLINK}
        )
    endif()
    zeron_copy_runtime_dependencies(${target})
endmacro()

# Add a new project asset directory that will be used by `zeron_copy_assets`
# `assetDir`: Full path to asset directory
# `outDir`: The relative path in project build output
function(zeron_add_project_asset_directory assetDir outDir)
    if(NOT IS_DIRECTORY ${assetDir})
        message(FATAL_ERROR "Asset directory is not valid: '${assetDir}'")
    endif()
    if(NOT EXISTS ${assetDir})
        message(FATAL_ERROR "Asset directory is not found: '${assetDir}'")
    endif()

    get_property(_projectAssets GLOBAL PROPERTY ZERON_PROJECT_ASSETS)
    set_property(GLOBAL PROPERTY ZERON_PROJECT_ASSETS "${_projectAssets}${assetDir}=${outDir};")
endfunction()

# Copy engine assets into build output of the project target
# `target`: The target to copy assets for
# `outputDir`: The relative directory in the build output folder the the assets will be copied into 
function(zeron_copy_assets target outputDir)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "Target does not exist!")
    endif()

    get_property(_buildAssets GLOBAL PROPERTY ZERON_BUILD_ASSETS_DIR)
    # Default to target build output directory
    if(NOT EXISTS ${_buildAssets})
        set(_buildAssets $<TARGET_FILE_DIR:${target}>)
    endif()

    # Zeron Engine Assets
    get_property(_engineAssetsdir GLOBAL PROPERTY ZERON_ASSETS_DIR)
    if(NOT EXISTS ${_engineAssetsdir})
        message(FATAL_ERROR "Zeron assets directory does not exist!")
    endif()
    add_custom_command(
        TARGET ${target} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "ZERON - Copying engine assets to '${_buildAssets}/${outputDir}' directory..."
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${_engineAssetsdir}" "${_buildAssets}/${outputDir}"
    )

    # Zeron Project Assets
    get_property(_projectAssets GLOBAL PROPERTY ZERON_PROJECT_ASSETS)
    foreach(_projectAssetInfo ${_projectAssets})
        string(FIND "${_projectAssetInfo}" "=" _assetIndex)
        if (_projectAssetInfo LESS 1)
            message(FATAL_ERROR "Invalid project asset directory information")
        endif()

        string(SUBSTRING ${_projectAssetInfo} 0 ${_assetIndex} _projectAssetDir)
        math(EXPR _assetIndex "${_assetIndex} + 1")  # Skip the separator
        string(SUBSTRING ${_projectAssetInfo} ${_assetIndex} -1 _projectAssetOutDir)

        if(EXISTS ${_projectAssetDir})
            add_custom_command(
                TARGET ${target} 
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "ZERON - Copying project assets from '${_projectAssetDir}' to '${_buildAssets}/${_projectAssetOutDir}'..."
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${_projectAssetDir}" "${_buildAssets}/${_projectAssetOutDir}"
            )
        else()
            message(FATAL_ERROR "Project asset directory doesn't exist: ${_projectAssetDir}")
        endif()
    endforeach()

endfunction()

# Configure the current directory with Zeron Engine compile options and definitions
macro(zeron_configure_compiler)
    if(NOT DEFINED ZERON_COMPILE_DEFINITIONS)
        message(FATAL_ERROR "Expected to find Zeron compile definitions!")
    endif()
    add_compile_definitions(${ZERON_COMPILE_DEFINITIONS})
    if(NOT DEFINED ZERON_COMPILE_OPTIONS)
        message(FATAL_ERROR "Expected to find Zeron compile options!")
    endif()
    add_compile_options(${ZERON_COMPILE_OPTIONS})
endmacro()

# Sets the directory that be used to output binary files
# `dirName`: The relative directory path from project build folder ("Build/<flavor>/Binaries/<dirName>")
macro(zeron_set_output_directory dirName)
    if(NOT DEFINED ${ZERON_BINARY_OUTPUT_DIRECTORY})
        set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries")
    endif()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}/${dirName}/Debug/)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}/${dirName}/Debug/)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}/${dirName}/Debug/)
    
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/${dirName}/Release/)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/${dirName}/Release/)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/${dirName}/Release/)
endmacro()

# Copies DLL dependencies to executable folder for Windows builds
# `target`: Target to copy DLLs for
macro(zeron_copy_runtime_dependencies target)
    if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E $<IF:$<BOOL:$<TARGET_RUNTIME_DLLS:${target}>>,copy,true> $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
            COMMAND_EXPAND_LISTS
        )
    endif()
endmacro()
