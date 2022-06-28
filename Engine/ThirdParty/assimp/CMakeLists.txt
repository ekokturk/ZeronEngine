# Copyright (C) Eser Kokturk. All Rights Reserved.

# assimp - Asset Import Library

set(ASSIMP_BUILD_ASSIMP_TOOLS   OFF)
set(ASSIMP_BUILD_TESTS          OFF)
set(ASSIMP_INSTALL              OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS   OFF)

set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")
add_subdirectory(.submodule)

set(ASSIMP_FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/assimp")
set_target_properties(assimp PROPERTIES FOLDER ${ASSIMP_FOLDER})
if(TARGET zlibstatic)
    set_target_properties(zlibstatic PROPERTIES FOLDER ${ASSIMP_FOLDER})
endif()
if(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs)
    set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER ${ASSIMP_FOLDER})
endif()