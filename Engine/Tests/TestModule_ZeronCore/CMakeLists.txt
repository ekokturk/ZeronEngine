# Copyright (C) Eser Kokturk. All Rights Reserved.

# --------- Zeron Core Tests --------- 
if(TARGET ZeronCore)
    zeron_add_module_test(TestModule_ZeronCore)
    target_link_libraries(
        TestModule_ZeronCore 
            PUBLIC 
                ZeronTestFramework
                ZeronCore 
    )
endif()