# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ THIRD PARTY ==============
add_zeron_module_dependency(googletest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(ZeronTestFramework)

add_subdirectory(TestModule_ZeronCore)
add_subdirectory(TestModule_ZeronRenderer)