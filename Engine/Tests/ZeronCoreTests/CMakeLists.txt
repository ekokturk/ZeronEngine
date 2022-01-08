# Copyright (C) Eser Kokturk. All Rights Reserved.

# --------- Zeron Core Tests --------- 
add_zeron_module_test(ZeronCoreTests)

target_link_libraries(
    ZeronCoreTests 
        PUBLIC 
            ZeronTestFramework
            ZeronCore 
)

