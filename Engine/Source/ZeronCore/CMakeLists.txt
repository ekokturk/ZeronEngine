# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronCore ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(fmt)
zeron_add_module_dependency(glm)
zeron_add_module_dependency(cereal)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")

if(ANDROID)
	find_library(ANDROID_LOG_LIB log)
	list(APPEND MODULE_LIBS_PRIVATE ${ANDROID_LOG_LIB})
endif()

# ============ MODULE INIT ============
target_include_directories(ZeronCore 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ZeronCore
	PUBLIC 
		fmt::fmt-header-only
		glm-header-only
		cereal
	PRIVATE
		${MODULE_LIBS_PRIVATE}
)

target_precompile_headers(ZeronCore	
	PUBLIC "<Common/PCH.h>"
)