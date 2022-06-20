# Copyright (C) Eser Kokturk. All Rights Reserved.

# ============ THIRD PARTY ==============
add_zeron_module_dependency(fmt)
add_zeron_module_dependency(glm)

# ============ MODULE CREATE ============
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Source")
add_zeron_module(ZeronCore ${SOURCE_DIRECTORY})

# ========= MODULE DEPENDENCIES =========

set(PCH_GENERIC 			
	<cstdint> <array> <map> <unordered_map> <queue> <any> <variant>
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
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(ZeronCore PUBLIC ZE_PLATFORM_LINUX=1)
endif()

if(MSVC)
 # Enable parallel builds
 target_compile_options(ZeronCore PUBLIC "/MP")
endif()

# ============ MODULE INIT ============
target_include_directories(ZeronCore 
    PUBLIC 
        "${SOURCE_DIRECTORY}"
)

target_link_libraries(ZeronCore
	PUBLIC 
		fmt::fmt-header-only
		glm-header-only
)

target_precompile_headers(ZeronCore	
    PUBLIC 
        ${PCH_GENERIC}
		${PCH_CORE}
)