# Copyright (C) Eser Kokturk. All Rights Reserved.

# --------- Zeron Renderer Tests --------- 
add_zeron_module_test(ZeronRendererTests)

target_link_libraries(
    ZeronRendererTests 
        PUBLIC 
            ZeronTestFramework
            ZeronRenderer 
)

