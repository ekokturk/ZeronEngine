# Copyright (C) Eser Kokturk. All Rights Reserved.

# --------- Zeron Core Tests --------- 
if(TARGET ZeronPhysics)
    zeron_add_module_test(TestModule_ZeronPhysics)
    target_link_libraries(
        TestModule_ZeronPhysics
            PUBLIC 
                ZeronTestFramework
                ZeronCore
                ZeronPhysics
    )
endif()