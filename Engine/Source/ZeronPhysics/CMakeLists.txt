# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronPhysics ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(physx)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC "")
set(MODULE_INCLUDES "")


# ============ MODULE INIT ============
target_include_directories(ZeronPhysics 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Private/"
)

target_link_libraries(ZeronPhysics
	PUBLIC
		${MODULE_LIBS_PUBLIC}
	PRIVATE
        ZeronCore
		PhysXLibs
		${MODULE_LIBS_PRIVATE}
)