# Copyright (C) Eser Kokturk. All Rights Reserved.

# --------- Zeron Core Tests --------- 
if(TARGET ZeronDB)
    zeron_add_module_test(TestModule_ZeronDB)
    target_link_libraries(
        TestModule_ZeronDB
            PUBLIC 
                ZeronTestFramework
                ZeronCore
                ZeronPlatform
                ZeronDB
    )
endif()