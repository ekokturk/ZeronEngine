# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ MODULE CREATE ============
zeron_add_module(ZeronCore ${CMAKE_CURRENT_SOURCE_DIR})

# ============ THIRD PARTY ==============
zeron_add_module_dependency(fmt)
zeron_add_module_dependency(glm)

# ========= MODULE DEPENDENCIES =========
set(MODULE_LIBS_PRIVATE "")

set(PCH_GENERIC 			
	<cstdint> <array> <map> <unordered_map> <set> <queue> <any> <variant>
	<exception> <iostream> <sstream> <fstream> <functional>
	<memory> <filesystem> <algorithm> <cmath> <bitset>
)

set(PCH_CORE
	"<Core/Core.h>"
	"<Core/Math/Math.h>"
)

if(WIN32)
    target_compile_definitions(ZeronCore PUBLIC ZE_PLATFORM_WIN32=1)
    list(APPEND PCH_GENERIC <Windows.h>)
elseif(LINUX)
    target_compile_definitions(ZeronCore PUBLIC ZE_PLATFORM_LINUX=1)
elseif(ANDROID)
    target_compile_definitions(ZeronCore PUBLIC ZE_PLATFORM_ANDROID=1)
	find_library(ANDROID_LOG_LIB log)
	list(APPEND MODULE_LIBS_PRIVATE ${ANDROID_LOG_LIB})
endif()

if(MSVC)
 # Enable parallel builds
 target_compile_options(ZeronCore PUBLIC "/MP")
endif()

# ============ MODULE INIT ============
target_include_directories(ZeronCore 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ZeronCore
	PUBLIC 
		fmt::fmt-header-only
		glm-header-only
	PRIVATE
		${MODULE_LIBS_PRIVATE}
)

target_precompile_headers(ZeronCore	
    PUBLIC 
        ${PCH_GENERIC}
		${PCH_CORE}
)