# Copyright (C) Eser Kokturk. All Rights Reserved.

# GLSLC - Shader compiler

# Try to find the installed shader compiler before downloading it
find_program(_glslc NAMES glslc HINTS Vulkan::glslc)

if(_glslc AND EXISTS ${_glslc})
    set(ZE_GLSLC_EXEC ${_glslc} CACHE INTERNAL "")
else()

    if(WIN32 OR ANDROID)
        set(GLSLC_WINDOWS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/windows/glslc.exe)
        if(NOT EXISTS ${GLSLC_WINDOWS_PATH})
            execute_process(COMMAND ${ZERON_PYTHON_EXE} update.py Windows WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
         endif()
        set(GLSLC_EXEC_PATH ${GLSLC_WINDOWS_PATH})
    elseif(LINUX)
        set(GLSLC_LINUX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/glslc)
        if(NOT EXISTS ${GLSLC_LINUX_PATH})
            execute_process(COMMAND ${ZERON_PYTHON_EXE} update.py Linux WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
        endif()
        set(GLSLC_EXEC_PATH ${GLSLC_LINUX_PATH})
    endif()

    set(ZE_GLSLC_EXEC ${GLSLC_EXEC_PATH} CACHE INTERNAL "")
endif()