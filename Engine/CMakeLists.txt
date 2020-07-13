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
	"${ENGINE_SOURCE_DIR}/ZeronEngine.cpp"
	"${ENGINE_SOURCE_DIR}/ZeronEngine.h"
	"${ENGINE_SOURCE_DIR}/Core/Private/Application.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Logger.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/CoreTypes/Color.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/CoreTypes/Vector2.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/CoreTypes/Vector3.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/Event.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/EventDispatcher.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/EventTypes/EventTypes.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/EventTypes/KeyEvents.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/EventTypes/MouseEvents.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Events/EventTypes/WindowEvents.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Input/Input.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Input/InputGLFW.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Input/InputModule.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Input/InputTypes/InputTypes.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Window/WindowContext.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Window/WindowContextGLFW.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Private/Window/WindowModule.cpp"
	"${ENGINE_SOURCE_DIR}/Core/Public/Application.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Core.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/IModule.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Logger.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/CoreTypes/Color.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/CoreTypes/CoreTypes.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/CoreTypes/Vector2.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/CoreTypes/Vector3.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/Event.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/EventDispatcher.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/EventTypes/EventTypes.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/EventTypes/KeyEvents.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/EventTypes/MouseEvents.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Events/EventTypes/WindowEvents.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Input/Input.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Input/InputGLFW.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Input/InputModule.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Input/InputTypes/InputTypes.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Window/WindowContext.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Window/WindowContextGLFW.h"
	"${ENGINE_SOURCE_DIR}/Core/Public/Window/WindowModule.h"
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
	"${ENGINE_SOURCE_DIR}/Renderer/Public"
	#ENGINE_INCLUDE_END

	PRIVATE
		"${ENGINE_THIRDPARTY_DIR}/GLFW/include"
		"${ENGINE_THIRDPARTY_DIR}/GLAD/include"
)

target_precompile_headers(
	ZeronEngine
  		PUBLIC
			<string>
			<unordered_map>
			<queue>
			<any>
			<variant>

			<iostream>
			<fstream>

			<functional>
			<memory>
)



target_link_libraries(
	ZeronEngine 
			glfw 
			glad 
			fmt::fmt-header-only
			
)
