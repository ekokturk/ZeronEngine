#
# This is the configuration script for installing executable target
#

# Exclude system dependencies
# - System files
# - Any library with so version
file(GET_RUNTIME_DEPENDENCIES 
    PRE_EXCLUDE_REGEXES 
        "api-ms-*" 
    POST_EXCLUDE_REGEXES
        ".*system32/.*\\.dll"
        "^.*\\.so\\..*$"
    RESOLVED_DEPENDENCIES_VAR _deps 
    EXECUTABLES "${_ZERON_INSTALL_TARGET_FILE_@_ZERON_INSTALL_TARGET@}"
)

# Install Executable
file(INSTALL DESTINATION @_ZERON_INSTALL_PATH@ TYPE EXECUTABLE FILES ${_deps})

# Install Resources
foreach(dir "${_ZERON_RESOURCE_PATH_LIST_@_ZERON_INSTALL_TARGET@}")
    file(INSTALL DESTINATION @_ZERON_INSTALL_PATH@ TYPE DIRECTORY FILES ${dir})
endforeach()

# Adjust RPath For linux shared libraries
set(_isLinux @LINUX@)
if(_isLinux)
    find_program(_patchelfExe patchelf HINTS "@ZERON_PYTHON_VENV_DIR@/bin")
    if (_patchelfExe)
        message("ZERON -- Modifying rpath for dependencies, patchelf found in ${_patchelfExe}")
        file(GLOB _installLibs "${CMAKE_INSTALL_PREFIX}/@_ZERON_INSTALL_TARGET@/*.so")
        foreach(_libPath ${_installLibs})
            if(NOT EXISTS ${_libPath})
                message(FATAL_ERROR "@ZERON_ERROR_MSG@ Unable to find library -> ${_libPath}")
            endif()
            get_filename_component(_libName ${_libPath} NAME)
            execute_process(
                COMMAND bash -c "${_patchelfExe} --set-rpath '$ORIGIN' '@_ZERON_INSTALL_PATH@/${_libName}'"
                WORKING_DIRECTORY "@_ZERON_INSTALL_PATH@"
                RESULT_VARIABLE _patchelfResult
                ERROR_VARIABLE _patchelfError
            )
            if (NOT _patchelfResult EQUAL 0)
                message(FATAL_ERROR "patchelf command failed with result: ${_patchelfResult}, error: ${_patchelfError}")
            endif()
        endforeach()
    else()
        message(WARNING "@ZERON_ERROR_MSG@ patchelf not found. Please install patchelf to proceed with RPath modifications.")
    endif()
endif()