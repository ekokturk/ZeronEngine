# Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

# ========== ENGINE DEPENDENCIES =========
add_subdirectory(ThirdParty)

# ========== ZERON ENGINE =========
set(ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(ENGINE_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set_output_directory("")

set(
    ENGINE_SOURCE_FILES
	#ENGINE_SOURCE_BEGIN
	"${ENGINE_SOURCE_DIR}/Core/Private/Application.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Input.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Logger.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Window.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/Event.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/EventDispatcher.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/EventTypes.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Public/Application.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Core.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Input.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Logger.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Window.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/Event.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/EventDispatcher.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/EventTypes.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/IEventHandler.h"
	"${ENGINE_SOURCE_DIR}/Utils/Public/Color.h"
	#ENGINE_SOURCE_END
)

source_group( TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ENGINE_SOURCE_FILES})

add_library( 
	ZeronEngine
	SHARED
		${ENGINE_SOURCE_FILES}
)

set_target_properties(ZeronEngine PROPERTIES FOLDER "ZeronEngine")

target_include_directories(ZeronEngine 
    PUBLIC 
		"${ENGINE_SOURCE_DIR}"
		#ENGINE_INCLUDE_BEGIN
	"${ENGINE_SOURCE_DIR}/Core/Public"
	"${ENGINE_SOURCE_DIR}/Utils/Public"
	#ENGINE_INCLUDE_END

	PRIVATE
		"${ENGINE_THIRDPARTY_DIR}/GLFW/include"
		"${ENGINE_THIRDPARTY_DIR}/GLAD/include"
)

target_link_libraries(
	ZeronEngine 
			glfw 
			glad 
			fmt::fmt-header-only
			
)
