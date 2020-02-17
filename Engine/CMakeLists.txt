# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

set(ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

set(
    ENGINE_SOURCE_FILES
	"${ENGINE_SOURCE_DIR}/Core/Private/main.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Application.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Public/Application.h"
)

source_group( TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source" FILES ${ENGINE_SOURCE_FILES})

add_executable(ZeronEngine ${ENGINE_SOURCE_FILES})
# add_library( ZeronEngine ${ENGINE_SOURCE_FILES})

target_include_directories(ZeronEngine 
    PUBLIC 
	"${ENGINE_SOURCE_DIR}/Core/Public"
)

