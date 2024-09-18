# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronGUI ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(ImGui)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")
set(MODULE_LIBS_PUBLIC imgui)
set(MODULE_INCLUDES "")


# ============ MODULE INIT ============
target_include_directories(ZeronGUI
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ZeronGUI
	PUBLIC
		${MODULE_LIBS_PUBLIC}
	PRIVATE
        ZeronRender
		ZeronPlatform
		${MODULE_LIBS_PRIVATE}
)