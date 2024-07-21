# Copyright (C) Eser Kokturk. All Rights Reserved.

if(NOT ZERON_ENABLE_SCRIPT)
    return()
endif()

# --------- Zeron Script Tests ---------
if(TARGET ZeronScript)
    zeron_add_module_test(TestModule_ZeronScript)
    set(LIBRARIES ZeronTestFramework ZeronCore ZeronScript)
    target_link_libraries(
        TestModule_ZeronScript
            PUBLIC 
                ${LIBRARIES}
    )
    zeron_copy_library_symbols(TestModule_ZeronScript "${LIBRARIES}")
endif()

