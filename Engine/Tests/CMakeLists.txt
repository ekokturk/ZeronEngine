# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ TESTS ==============
enable_testing()

if (NOT MSVC)
    include(CTest) 
    include(Dart)
endif()

message("ZERON - Generating: Zeron Engine Tests")

zeron_add_module_dependency(GoogleTest)

if(TARGET gtest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    include(GoogleTest)
    
    # Zeron Test Dependencies
    add_subdirectory(ZeronTestFramework)

    # Zeron Test Modules
    add_subdirectory(Modules/ZeronCore)
    add_subdirectory(Modules/ZeronPlatform)
    add_subdirectory(Modules/ZeronScript)
    add_subdirectory(Modules/ZeronPhysics)
    add_subdirectory(Modules/ZeronDB)
    add_subdirectory(Modules/ZeronNet)

    add_subdirectory(ZeronTestApp)
    
else()
    message(FATAL_ERROR "${ZERON_ERROR_MSG} Google Test library was not found!")
endif()