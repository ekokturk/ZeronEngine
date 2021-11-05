# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# ============ THIRD PARTY ==============
add_zeron_module_dependency(fmt)

# ============ MODULE CREATE ============
add_zeron_module(ZeronCore ${CMAKE_CURRENT_SOURCE_DIR})

# ========= MODULE DEPENDENCIES =========


set(PCH_GENERIC 			
	<string> <map> <unordered_map> <queue> <any> <variant>
	<exception> <iostream> <sstream> <fstream> <functional>
	<memory> <filesystem> <algorithm> <cmath> <bitset>
)

set(PCH_CORE
	# "${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.h"
	"Core/Core.h"
	"Core/Logger.h"
)

message(WARNING ${PCH_CORE})

if(WIN32)
    list(APPEND PCH_GENERIC <Windows.h>)
endif()

# ============ MODULE INIT ============
target_include_directories(ZeronCore 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(ZeronCore
	PUBLIC 
		fmt::fmt-header-only
)

target_precompile_headers(ZeronCore	
    PUBLIC 
        ${PCH_GENERIC}
		${PCH_CORE}
)