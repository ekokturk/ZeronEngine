// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_GLFW

#	include <Platform/API/Shared/GLFW/SystemEventProcessorGLFW.h>

#	include <GLFW/glfw3.h>
#	include <Platform/API/Shared/GLFW/GLFWHelpers.h>
#	include <Platform/API/Shared/GLFW/WindowGLFW.h>

namespace Zeron
{
	SystemEventProcessorGLFW::SystemEventProcessorGLFW(DispatchFn dispatchCallback, ContextFn contextCallback)
		: SystemEventProcessor(std::move(dispatchCallback), std::move(contextCallback))
	{}

	void SystemEventProcessorGLFW::ProcessEvents()
	{
		glfwPollEvents();
	}

	void SystemEventProcessorGLFW::registerEventCallbacks(WindowGLFW* window)
	{
		auto glfwWindow = static_cast<GLFWwindow*>(window->GetApiHandle());

		// ------------------------ WINDOW EVENTS --------------------------
		// WINDOW RESIZE
		glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* windowGLFW, int width, int height) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			const int canResize = glfwGetWindowAttrib(windowGLFW, GLFW_RESIZABLE);
			// Size of the window is not changed on minimize
			if (!ctx.mWindow->IsMinimized() && canResize == GLFW_TRUE) {
				processor->Dispatch({ SystemEvent::WindowResizeStarted{ width, height } }, ctx);
				processor->Dispatch({ SystemEvent::WindowSizeChanged{ width, height } }, ctx);
			}
		});

		// WINDOW REPOSITION
		glfwSetWindowPosCallback(glfwWindow, [](GLFWwindow* windowGLFW, int posX, int posY) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			// Here is ideal to handle Minimize/Maximize since this event is triggered before them
			if (glfwGetWindowAttrib(windowGLFW, GLFW_ICONIFIED) == GLFW_TRUE) {
				processor->Dispatch({ SystemEvent::WindowMinimized{} }, ctx);
				// Position of the window is not changed on minimize
				return;
			}
			if (ctx.mWindow->IsMinimized()) {
				if (ctx.mWindow->IsFullScreen()) {
					return;
				}
				processor->Dispatch({ SystemEvent::WindowRestored{} }, ctx);
			}
			if (glfwGetWindowAttrib(windowGLFW, GLFW_MAXIMIZED) == GLFW_TRUE) {
				processor->Dispatch({ SystemEvent::WindowMaximized{} }, ctx);
			}
			else {
				if (ctx.mWindow->IsMaximized()) {
					processor->Dispatch({ SystemEvent::WindowRestored{} }, ctx);
				}
			}
			processor->Dispatch({ SystemEvent::WindowMoved{ posX, posY } }, ctx);
		});

		// WINDOW CLOSED
		glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* windowGLFW) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			processor->Dispatch({ SystemEvent::WindowClosed{} }, ctx);
		});

		// WINDOW FOCUSED/UNFOCUSED
		glfwSetWindowFocusCallback(glfwWindow, [](GLFWwindow* windowGLFW, int isFocused) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			switch (isFocused) {
				case GLFW_TRUE: {
					processor->Dispatch({ SystemEvent::WindowFocused{} }, ctx);
				} break;
				case GLFW_FALSE: {
					processor->Dispatch({ SystemEvent::WindowUnfocused{} }, ctx);
				} break;
				default:;
			}
		});

		// glfwSetFramebufferSizeCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int scaleX, int scaleY) {
		//	WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
		// });

		// ------------------------ MOUSE EVENTS ---------------------------
		// MOUSE BUTTON PRESSED
		glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* windowGLFW, int buttonIndex, int actionType, int modifiers) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			double xPos, yPos;
			glfwGetCursorPos(windowGLFW, &xPos, &yPos);
			switch (actionType) {
				case GLFW_PRESS: {
					processor->Dispatch({ SystemEvent::MouseButtonDown{ GLFWHelpers::GetMouseCode(buttonIndex), static_cast<int>(xPos), static_cast<int>(yPos) } }, ctx);
				} break;
				case GLFW_RELEASE: {
					processor->Dispatch({ SystemEvent::MouseButtonUp{ GLFWHelpers::GetMouseCode(buttonIndex), static_cast<int>(xPos), static_cast<int>(yPos) } }, ctx);
				} break;
				default:;
			}
		});

		// MOUSE MOVED
		glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* windowGLFW, double posX, double posY) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			if (ctx.mWindow->IsHovered()) {
				processor->Dispatch({ SystemEvent::MouseMoved{ static_cast<int>(posX), static_cast<int>(posY) } }, ctx);
			}
		});

		// MOUSE SCROLLED
		glfwSetScrollCallback(glfwWindow, [](GLFWwindow* windowGLFW, double offsetX, double offsetY) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			processor->Dispatch({ SystemEvent::MouseScroll{ static_cast<float>(offsetX), static_cast<float>(offsetY) } }, ctx);
		});

		// MOUSE ENTER/EXIT
		glfwSetCursorEnterCallback(glfwWindow, [](GLFWwindow* windowGLFW, int isEntered) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			if (ctx.mWindow->IsFocused() && static_cast<WindowGLFW*>(ctx.mWindow)->IsCursorClipped()) {
				return;
			}
			switch (isEntered) {
				case GLFW_TRUE: {
					processor->Dispatch({ SystemEvent::MouseEnter{} }, ctx);
				} break;
				case GLFW_FALSE: {
					processor->Dispatch({ SystemEvent::MouseExit{} }, ctx);
				} break;
				default:;
			}
		});

		// --------------------------- KEY EVENTS --------------------------
		glfwSetKeyCallback(glfwWindow, [](GLFWwindow* windowGLFW, int keyIndex, int scanCode, int actionType, int modifierMask) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			const SystemEventContext ctx = processor->GetEventContext(windowGLFW);
			switch (actionType) {
				case GLFW_PRESS: {
					processor->Dispatch({ SystemEvent::KeyDown{ GLFWHelpers::GetKeyCode(keyIndex) } }, ctx);
				} break;
				case GLFW_RELEASE: {
					processor->Dispatch({ SystemEvent::KeyUp{ GLFWHelpers::GetKeyCode(keyIndex) } }, ctx);
				} break;
				default:;
			}
		});

		glfwSetCharModsCallback(glfwWindow, [](GLFWwindow* windowGLFW, unsigned int codepoint, int modifierMask) {
			const SystemEventProcessorGLFW* processor = GetUserPointer(windowGLFW);
			processor->Dispatch({ SystemEvent::TextChar{ static_cast<unsigned char>(codepoint) } }, processor->GetEventContext(windowGLFW));
		});
	}

	SystemEventProcessorGLFW* SystemEventProcessorGLFW::GetUserPointer(GLFWwindow* glfwWindow)
	{
		return static_cast<SystemEventProcessorGLFW*>(glfwGetWindowUserPointer(glfwWindow));
	}

	SystemEventContext SystemEventProcessorGLFW::GetEventContext(GLFWwindow* glfwWindow) const
	{
		return mContextCallback(GLFWHelpers::GetPlatformWindowHandle(glfwWindow));
	}

	void SystemEventProcessorGLFW::Dispatch(const SystemEvent& evt, const SystemEventContext& context) const
	{
		mDispatchCallback(evt, context);
	}
}

#endif