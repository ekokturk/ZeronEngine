# Copyright (C) Eser Kokturk. All Rights Reserved.

get_property(ZERON_THIRDPARTY_DIR GLOBAL PROPERTY ZERON_THIRDPARTY_DIR)

set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Source")
add_zeron_module(ZeronClient ${SOURCE_DIRECTORY})

target_include_directories(ZeronClient 
    PUBLIC 
        ${SOURCE_DIRECTORY}
	# PRIVATE
)

target_link_libraries(
	ZeronClient
		PUBLIC
			ZeronCore
            ZeronRenderer
)
