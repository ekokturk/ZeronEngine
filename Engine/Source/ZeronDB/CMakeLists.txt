# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronDB ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(lmdb)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")


# ============ MODULE INIT ============
target_include_directories(ZeronDB 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Private/"
)

target_link_libraries(ZeronDB
	PUBLIC
		${MODULE_LIBS_PUBLIC}
	PRIVATE
        ZeronCore
		lmdb
		${MODULE_LIBS_PRIVATE}
)