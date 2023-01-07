# Copyright (C) Eser Kokturk. All Rights Reserved.

# --------- Zeron Renderer Tests --------- 
if(TARGET ZeronPlatform)
    zeron_add_module_test(TestModule_ZeronPlatform)
    target_link_libraries(
        TestModule_ZeronPlatform
            PUBLIC 
                ZeronTestFramework
                ZeronPlatform 
    )
endif()