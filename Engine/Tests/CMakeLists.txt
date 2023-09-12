# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ THIRD PARTY ==============
if (NOT MSVC)
    include(CTest) 
    include(Dart)
endif()

message("ZERON - Generating: Zeron Engine Tests")

zeron_add_module_dependency(GoogleTest)

if(TARGET gtest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Zeron Test Dependencies
    add_subdirectory(ZeronTestFramework)

    # Zeron Test Modules
    add_subdirectory(TestModule_ZeronCore)
    add_subdirectory(TestModule_ZeronPlatform)
else()
    message(FATAL_ERROR "${ZERON_ERROR_MSG} Google Test library was not found!")
endif()