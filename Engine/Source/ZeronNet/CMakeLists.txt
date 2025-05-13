# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronNet ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(asio)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")


# ============ MODULE INIT ============
target_include_directories(ZeronNet 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Private/"
)

target_link_libraries(ZeronNet
	PUBLIC
		${MODULE_LIBS_PUBLIC}
	PRIVATE
        ZeronCore
		asio
		${MODULE_LIBS_PRIVATE}
)