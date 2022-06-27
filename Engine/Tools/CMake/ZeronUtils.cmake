
function(zeron_copy_assets outputDir)
    get_property(assets_dir GLOBAL PROPERTY ZERON_ASSETS_DIR)
    if(NOT EXISTS ${assets_dir})
        message(FATAL_ERROR "Zeron assets directory does not exist!")
    endif()
    add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${assets_dir}" "${outputDir}"
        COMMENT "Copying Zeron assets to '${outputDir}' directory..."
    )
endfunction()
