# Copyright (C) Eser Kokturk. All Rights Reserved.

# Physx - Physics Library

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")

set(_physxTargets 
    PhysXFoundation 
    PhysXCommon 
    LowLevel 
    LowLevelAABB 
    LowLevelDynamics 
    PhysXTask
    SceneQuery
    SimulationController
    PhysXPvdSDK
    FastXml
    PhysXExtensions
    PhysX
)

# Configure platform and properties
if(WIN32)
    set(TARGET_BUILD_PLATFORM windows)
    set(RESOURCE_LIBPATH_SUFFIX x64)
elseif(LINUX)
    set(TARGET_BUILD_PLATFORM linux)
elseif(ANDROID)
    set(TARGET_BUILD_PLATFORM android)
endif()

set(PHYSX_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.submodule/physx)
set(PROJECT_CMAKE_FILES_DIR source/compiler/cmake)
set(PX_GENERATE_STATIC_LIBRARIES TRUE)
set(PUBLIC_RELEASE TRUE)

foreach(_physxTarget ${_physxTargets})
    # Add predefined sources for libraries
    include(".submodule/physx/source/compiler/cmake/${_physxTarget}.cmake")
    set_target_properties(${_physxTarget} PROPERTIES FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/PhysX")
    target_compile_definitions(${_physxTarget} PUBLIC PX_PHYSX_STATIC_LIB=1 $<$<CONFIG:Debug>:_DEBUG>)
endforeach()

add_library(PhysXLibs INTERFACE)
target_link_libraries(PhysXLibs INTERFACE ${_physxTargets})

