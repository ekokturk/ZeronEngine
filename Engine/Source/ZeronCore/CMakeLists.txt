# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

add_zeron_module(ZeronCore ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(ZeronCore 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(PCH_GENERIC 			
	<string> <map> <unordered_map> <queue> <any> <variant>
	<exception> <iostream> <sstream> <fstream> <functional>
	<memory> <filesystem>
)

target_precompile_headers(ZeronCore	
    PUBLIC 
        ${PCH_GENERIC}
)

