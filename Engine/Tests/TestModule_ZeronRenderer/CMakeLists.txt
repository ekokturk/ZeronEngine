# Copyright (C) Eser Kokturk. All Rights Reserved.

# --------- Zeron Renderer Tests --------- 
if(TARGET ZeronRenderer)
    add_zeron_module_test(TestModule_ZeronRenderer)
    target_link_libraries(
        TestModule_ZeronRenderer 
            PUBLIC 
                ZeronTestFramework
                ZeronRenderer 
    )
endif()