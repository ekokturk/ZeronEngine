# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# ============ THIRD PARTY ==============
add_zeron_module_dependency(googletest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(ZeronTestFramework)

add_subdirectory(ZeronCoreTests)
add_subdirectory(ZeronRendererTests)