
function(zeron_copy_assets target outputDir)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "Target does not exist!")
    endif()
    get_property(assets_dir GLOBAL PROPERTY ZERON_ASSETS_DIR)
    if(NOT EXISTS ${assets_dir})
        message(FATAL_ERROR "Zeron assets directory does not exist!")
    endif()
    get_property(build_assets GLOBAL PROPERTY ZERON_BUILD_ASSETS_DIR)
    if(NOT EXISTS ${build_assets})
        set(build_assets $<TARGET_FILE_DIR:${target}>)
    endif()
    add_custom_command(
        TARGET ${target} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying Zeron assets to '${build_assets}/${outputDir}' directory..."
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${assets_dir}" "${build_assets}/${outputDir}"
    )
endfunction()
