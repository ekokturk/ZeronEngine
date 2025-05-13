# Copyright (C) Eser Kokturk. All Rights Reserved.

# --------- Zeron Core Tests --------- 
if(TARGET ZeronNet)
    zeron_add_module_test(TestModule_ZeronNet)
    target_link_libraries(
        TestModule_ZeronNet
            PUBLIC 
                ZeronTestFramework
                ZeronCore
                ZeronNet
    )
endif()