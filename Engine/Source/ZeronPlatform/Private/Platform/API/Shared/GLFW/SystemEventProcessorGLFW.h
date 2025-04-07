// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_WINDOW_GLFW

#	include <Platform/SystemEventProcessor.h>

struct GLFWwindow;

namespace Zeron
{
	class WindowGLFW;
	class Platform;

	class SystemEventProcessorGLFW : public SystemEventProcessor {
	  public:
		SystemEventProcessorGLFW(DispatchFn dispatchCallback, ContextFn contextCallback);

		void ProcessEvents() override;

		void registerEventCallbacks(WindowGLFW* window);

		static SystemEventProcessorGLFW* GetUserPointer(GLFWwindow* glfwWindow);

		SystemEventContext GetEventContext(GLFWwindow* glfwWindow) const;
		void Dispatch(const SystemEvent& evt, const SystemEventContext& context) const;
	};
}

#endif