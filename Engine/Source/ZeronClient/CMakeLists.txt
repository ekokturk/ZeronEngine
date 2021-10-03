# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

get_property(ZERON_THIRDPARTY_DIR GLOBAL PROPERTY ZERON_THIRDPARTY_DIR)

add_zeron_module(ZeronClient ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(ZeronClient 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}"
	# PRIVATE
)

target_link_libraries(
	ZeronClient
		PUBLIC
			ZeronCore
            ZeronRenderer
)
