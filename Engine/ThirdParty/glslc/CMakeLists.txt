# Copyright (C) Eser Kokturk. All Rights Reserved.

# GLSLC - Shader compiler

# Try to find the installed shader compiler before downloading it
find_program(_glslc NAMES glslc HINTS Vulkan::glslc)

if(_glslc AND EXISTS ${_glslc})
    set(GLSLC_EXEC ${_glslc} CACHE INTERNAL "")
else()
    set(GLSLC_WINDOWS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/windows/glslc.exe)
    set(GLSLC_LINUX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/glslc)

    if((NOT EXISTS ${GLSLC_WINDOWS_PATH}) OR (NOT EXISTS ${GLSLC_LINUX_PATH}))
        file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
        execute_process(COMMAND python update.py WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    endif()
    if(LINUX)
        set(GLSLC_EXEC ${GLSLC_LINUX_PATH} CACHE INTERNAL "")
    else()
        set(GLSLC_EXEC ${GLSLC_WINDOWS_PATH} CACHE INTERNAL "")
    endif()
endif()