// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "ZeronEngine.h"

#include "Window/WindowContextGLFW.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Events/EventDispatcher.h"
#include "Events/EventTypes/EventTypes.h"
#include "Input/InputTypes/InputTypesGLFW.h"

//#define DEBUG_WINDOW_CONTEXT 
#ifdef DEBUG_WINDOW_CONTEXT 
	#define DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS 
	//#define DEBUG_WINDOW_CONTEXT_KEY_EVENTS 
	#define DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS 
#endif

namespace ZeronEngine
{
	// Count of active GLFW window contexes
	int WindowContextGLFW::s_WindowCount = 0;
	
	bool WindowContextGLFW::ConfigureContext()
	{
		if(s_WindowCount > 0){
			return true;
		}

		// OpenGL context
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Initialize GLFW
		if (glfwInit() == false)
		{
			ZERON_LOG_ERROR("GLFW failed to initialized!")
			return false;
		}

		// Set error message callback for GLFW
		glfwSetErrorCallback([](int errorCode, const char* errorMessage)
		{
			ZERON_LOG_ERROR("GLFW Error {}: {}", errorCode, errorMessage)
		});
		
		return true;
	}

	WindowContextGLFW::WindowContextGLFW(WindowProps&& windowProps)
		: WindowContext(std::move(windowProps)),
		m_WindowHandle(nullptr)
	{
		// Create Window
		m_WindowHandle = glfwCreateWindow(m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.Name.c_str(), nullptr, nullptr);
		if (m_WindowHandle == nullptr)
		{
			ZERON_LOG_ERROR("GLFW failed to create window context '{}'!", m_WindowProps.Name)
			WindowContextGLFW::~WindowContextGLFW();
			return;
		}
		glfwMakeContextCurrent(m_WindowHandle);
		// Initialize glad
		if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
		{
			ZERON_LOG_ERROR("GLAD initialization failed for window context '{}'!", m_WindowProps.Name)
			return;
		}

		s_WindowCount++;
		glfwSetWindowUserPointer(m_WindowHandle, this);
		
		ZERON_LOG_INFO("Window '{}' is created.", GetName());
	}

	WindowContextGLFW::~WindowContextGLFW()
	{
		#ifdef DEBUG_WINDOW_CONTEXT
			ZERON_LOG("Window Context GLFW : Window context is destroyed...", m_WindowProps.Name)
		#endif
		glfwDestroyWindow(m_WindowHandle);
		m_WindowProps.EventDispatcher->Remove(this);
		s_WindowCount--;
		if(s_WindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowContextGLFW::Update()
	{
		if(!glfwWindowShouldClose(m_WindowHandle))
		{
			glfwSwapBuffers(m_WindowHandle);
			glfwPollEvents();
		}
	}

	void WindowContextGLFW::Destroy()
	{
		WindowContextGLFW::~WindowContextGLFW();
	}

	void WindowContextGLFW::SetVisible()
	{
		glfwShowWindow(m_WindowHandle);
	}

	void WindowContextGLFW::SetHidden()
	{
		glfwHideWindow(m_WindowHandle);
	}
	
	void WindowContextGLFW::SetName(const std::string& name)
	{
		m_WindowProps.Name = name;
		glfwSetWindowTitle(m_WindowHandle, m_WindowProps.Name.c_str());
	}

	void WindowContextGLFW::SetAspectRatio(int numerator, int denominator)
	{
		ZERON_LOG_INFO("Window '{}' aspect ratio changed to {}:{}", GetName(), numerator, denominator);
		glfwSetWindowAspectRatio(m_WindowHandle, numerator, denominator);
	}

	void WindowContextGLFW::SetSize(int width, int height)
	{
		ZERON_LOG_INFO("Window '{}' resized to {}px-{}px", GetName(), width, height);
		m_WindowProps.Width = width;
		m_WindowProps.Height = height;
		glfwSetWindowSize(m_WindowHandle, width, height);
	}

	void WindowContextGLFW::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight)
	{
		glfwSetWindowSizeLimits(m_WindowHandle, minWidth, minHeight, maxWidth, maxHeight);
	}

	void WindowContextGLFW::SetMousePosition(const Vector2& mousePosition)
	{
		m_WindowProps.MousePosition.X = mousePosition.X;
		m_WindowProps.MousePosition.Y = mousePosition.Y;
		
		glfwSetCursorPos(m_WindowHandle, m_WindowProps.MousePosition.X, m_WindowProps.MousePosition.Y);
	}

	void WindowContextGLFW::SetMinimized()
	{
		glfwIconifyWindow(m_WindowHandle);
	}

	void WindowContextGLFW::SetMaximized()
	{
		glfwMaximizeWindow(m_WindowHandle);
	}

	void WindowContextGLFW::SetRestored()
	{
		glfwRestoreWindow(m_WindowHandle);
	}

	void WindowContextGLFW::SetFocused()
	{
		glfwFocusWindow(m_WindowHandle);
	}

	void WindowContextGLFW::SetAttention()
	{
		glfwRequestWindowAttention(m_WindowHandle);
	}

	void WindowContextGLFW::RegisterEvents()
	{
		// TODO glfwSetCharCallback

	// -----------------------------------------------------------------
	// ------------------------ MOUSE EVENTS ---------------------------
	// -----------------------------------------------------------------

		// MOUSE BUTTON PRESSED
		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, int buttonIndex, int actionType, int modifiers)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const MouseCode mouseCode = InputHelpers::GetMouseCodeFromGLFW(buttonIndex);
			const ModifierKeys modifierKeys = ModifierKeys(modifiers);
			switch (actionType)
			{
				case GLFW_PRESS:{
					const Events::Mouse::Press e(mouseCode, modifierKeys, window->GetWindowContextHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Press>(e);
					break;
				}
				case GLFW_RELEASE:{
					const Events::Mouse::Release e(mouseCode, modifierKeys, window->GetWindowContextHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Release>(e);
					break;
				}
				default:;
			}
		});
	
		
		// MOUSE ENTER/EXIT
		glfwSetCursorEnterCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, int isEntered)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			if(isEntered == GLFW_TRUE){
				const Events::Mouse::Enter e(window->GetWindowContextHandle());
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Enter>(e);
				#ifdef DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS
					ZERON_LOG_INFO("Window Context GLFW : Mouse cursor entered window '{}'", window->GetName())
				#endif
			}else{
				const Events::Mouse::Exit e(window->GetWindowContextHandle());
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Exit>(e);
				#ifdef DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS
					ZERON_LOG_INFO("Window Context GLFW : Mouse cursor exited window '{}'", window->GetName())
				#endif
			}
		});
		
		// MOUSE MOVED
		glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, double posX, double posY)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			window->m_WindowProps.MousePosition.X = static_cast<float>(posX);
			window->m_WindowProps.MousePosition.Y = static_cast<float>(posY);
			const Events::Mouse::Move e(static_cast<float>(posX), static_cast<float>(posY));
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Move>(e);
			#ifdef DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS
				ZERON_LOG_INFO("Window Context GLFW : Mouse moved X:{} | Y:{}", posX, posY)
			#endif
		});

		// MOUSE SCROLLED
		glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, double offsetX, double offsetY)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const Events::Mouse::Scroll e(static_cast<float>(offsetX), static_cast<float>(offsetY));
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Scroll>(e);
			#ifdef DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS
				ZERON_LOG_INFO("Window Context GLFW : Mouse Scrolled {} {}", offsetX, offsetY)
			#endif
		});

	// -----------------------------------------------------------------
	// --------------------------- KEY EVENTS --------------------------
	// -----------------------------------------------------------------
		
		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, int keyIndex, int scanCode, int actionType, int modifierMask)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const ModifierKeys modifierKeys = ModifierKeys(modifierMask);
			const KeyCode keyCode = KeyCode(InputHelpers::GetKeyCodeFromGLFW(keyIndex));
			switch (actionType)
			{
				case GLFW_PRESS:{
					const Events::Key::Press e(keyCode, modifierKeys, window->GetWindowContextHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Key::Press>(e);
					break;
				}
				case GLFW_RELEASE:{
					const Events::Key::Release e(keyCode, modifierKeys, window->GetWindowContextHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Key::Release>(e);
					break;
				}
				case GLFW_REPEAT:{
					const Events::Key::Repeat e(keyCode, modifierKeys, window->GetWindowContextHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Key::Repeat>(e);
					break;
				}
				default:;
			}
		});

		glfwSetCharModsCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, unsigned int codepoint, int modifierMask)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const ModifierKeys modifierKeys = ModifierKeys(modifierMask);
			Events::Key::Character e(codepoint, modifierKeys, window->GetWindowContextHandle());
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Key::Character>(e);
		});



		
		// -----------------------------------------------------------------
		// ----------------------- GAMEPAD EVENTS --------------------------
		// -----------------------------------------------------------------

		//glfwSetJoystickCallback()

		
		// -----------------------------------------------------------------
		// ------------------------ WINDOW EVENTS --------------------------
		// -----------------------------------------------------------------

		// WINDOW RESIZE
		glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, int width, int height)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			window->m_WindowProps.Height = height;
			window->m_WindowProps.Width = width;
			const Events::Window::Resize e(width,height);
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Resize>(e);
			#ifdef DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
				ZERON_LOG_INFO("Window Context GLFW : Window '{}' resized to {}px-{}px", window->GetName(), width, width);
			#endif
		});
		
		// WINDOW CLOSED
		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* windowGLFW)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const Events::Window::Close e(window->GetWindowContextHandle());
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Close>(e);
			#ifdef DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
				ZERON_LOG_INFO("Window Context GLFW : Window '{}' closed", window->GetName());
			#endif
		});

		// WINDOW FOCUSED/UNFOCUSED
		glfwSetWindowFocusCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, int isFocused)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			if(isFocused == GLFW_TRUE){
				const Events::Window::Focus e;
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Focus>(e);
				#ifdef DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
					ZERON_LOG_INFO("Window Context GLFW : Window '{}' focused", window->GetName());
				#endif
			}else{
				const Events::Window::Unfocus e;
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Unfocus>(e);
				#ifdef DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
					ZERON_LOG_INFO("Window Context GLFW : Window '{}' unfocused", window->GetName());
				#endif
			}
		});

		// WINDOW MINIMIZED/RESTORED
		glfwSetWindowIconifyCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, int isMinimized)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			if(isMinimized == GLFW_TRUE){
				const Events::Window::Minimize e;
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Minimize>(e);
			}else{
				const Events::Window::Restore e;
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Restore>(e);
			}
		});

		// WINDOW MAXIMIZED/RESTORED
		glfwSetWindowMaximizeCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, int isMaximized)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			if (isMaximized == GLFW_TRUE) {
				const Events::Window::Maximize e;
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Maximize>(e);
			}else {
				const Events::Window::Restore e;
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Restore>(e);
			}
		});
		
	}

}
