// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "ZeronEngine.h"

#include "Window/WindowGLFW.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "GLFWHelpers.h"
#include "Events/EventDispatcher.h"
#include "Events/EventTypes/EventTypes.h"

#define DEBUG_WINDOW_CONTEXT 
#ifdef DEBUG_WINDOW_CONTEXT 
	//#define DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS 
	#define DEBUG_WINDOW_CONTEXT_KEY_EVENTS 
	#define DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS 
#endif

namespace ZeronEngine
{
	// Count of active GLFW window contexes
	int WindowGLFW::s_WindowCount = 0;
	
	bool WindowGLFW::ConfigureContext()
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

	WindowGLFW::WindowGLFW(WindowProps&& windowProps)
		: Window(std::move(windowProps)),
		m_WindowGLFW(nullptr)
	{
		// Initialize window creation from window properties
		m_MonitorGLFW = m_WindowProps.IsFullScreen ? GetCurrentMonitor() : nullptr;
		const GLFWvidmode* videoMode = m_WindowProps.IsFullScreen ? glfwGetVideoMode(m_MonitorGLFW) : nullptr;
		const int initWidth = m_WindowProps.IsFullScreen ? videoMode->width : m_WindowProps.Width;
		const int initHeight = m_WindowProps.IsFullScreen ? videoMode->height : m_WindowProps.Height;
		const int initRefreshRate = m_WindowProps.IsFullScreen ? videoMode->refreshRate : m_WindowProps.Width;

		// Set refresh rate
		glfwWindowHint(GLFW_REFRESH_RATE, initRefreshRate);

		// Create Window
		m_WindowGLFW = glfwCreateWindow(initWidth, initHeight, m_WindowProps.Name.c_str(), m_MonitorGLFW, nullptr);
		if (m_WindowGLFW == nullptr)
		{
			ZERON_LOG_ERROR("GLFW failed to create Window '{}'!", m_WindowProps.Name)
			WindowGLFW::~WindowGLFW();
			return;
		}
		glfwMakeContextCurrent(m_WindowGLFW);
		
		// Initialize glad
		if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
		{
			ZERON_LOG_ERROR("GLAD initialization failed for Window '{}'!", m_WindowProps.Name)
			return;
		}

		s_WindowCount++;
		glfwSetWindowUserPointer(m_WindowGLFW, this);
		
		ZERON_LOG_INFO("Window '{}' is created.", GetName());


	}

	WindowGLFW::~WindowGLFW()
	{
		#ifdef DEBUG_WINDOW_CONTEXT
			ZERON_LOG("Window GLFW : Window is destroyed...", m_WindowProps.Name)
		#endif
		glfwDestroyWindow(m_WindowGLFW);
		m_WindowProps.EventDispatcher->Remove(this);
		s_WindowCount--;
		if(s_WindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowGLFW::Update()
	{
		if(!glfwWindowShouldClose(m_WindowGLFW))
		{		
			glfwSwapBuffers(m_WindowGLFW);
			glfwPollEvents();
		}
	}

	void WindowGLFW::Destroy()
	{
		WindowGLFW::~WindowGLFW();
	}

	void WindowGLFW::SetVisible()
	{
		glfwShowWindow(m_WindowGLFW);
	}

	void WindowGLFW::SetHidden()
	{
		glfwHideWindow(m_WindowGLFW);
	}
	
	void WindowGLFW::SetName(const std::string& name)
	{
		m_WindowProps.Name = name;
		glfwSetWindowTitle(m_WindowGLFW, m_WindowProps.Name.c_str());
	}

	void WindowGLFW::SetAspectRatio(int numerator, int denominator)
	{
		ZERON_LOG_INFO("Window '{}' aspect ratio changed to {}:{}", GetName(), numerator, denominator);
		glfwSetWindowAspectRatio(m_WindowGLFW, numerator, denominator);
	}

	void WindowGLFW::SetSize(int width, int height)
	{
		ZERON_LOG_INFO("Window '{}' resized to {}px-{}px", GetName(), width, height);
		m_WindowProps.Width = width;
		m_WindowProps.Height = height;
		glfwSetWindowSize(m_WindowGLFW, width, height);
	}

	void WindowGLFW::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight)
	{
		glfwSetWindowSizeLimits(m_WindowGLFW, minWidth, minHeight, maxWidth, maxHeight);
	}

	void WindowGLFW::SetMousePosition(const Vector2& mousePosition)
	{
		m_WindowProps.MousePosition.X = mousePosition.X;
		m_WindowProps.MousePosition.Y = mousePosition.Y;
		
		glfwSetCursorPos(m_WindowGLFW, m_WindowProps.MousePosition.X, m_WindowProps.MousePosition.Y);
	}

	void WindowGLFW::SetFullScreen(bool isFullScreen)
	{
		m_WindowProps.IsFullScreen = isFullScreen;

		if(isFullScreen)
		{
			m_MonitorGLFW = GetCurrentMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(m_MonitorGLFW);
			glfwSetWindowMonitor(m_WindowGLFW, m_MonitorGLFW, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			m_MonitorGLFW = GetCurrentMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(m_MonitorGLFW);
			glfwSetWindowMonitor(m_WindowGLFW, nullptr, 0, 0, m_WindowProps.WidthPrev, m_WindowProps.HeightPrev, mode->refreshRate);
		}
	}

	GLFWmonitor* WindowGLFW::GetCurrentMonitor() const
	{
		// If window is already created, find the monitor of that window lives in
		if(m_MonitorGLFW != nullptr)
		{
			
		}
		
		return glfwGetPrimaryMonitor();
	}

	void WindowGLFW::SetMinimized()
	{
		glfwIconifyWindow(m_WindowGLFW);
	}

	void WindowGLFW::SetMaximized()
	{
		glfwMaximizeWindow(m_WindowGLFW);
	}

	void WindowGLFW::SetRestored()
	{
		glfwRestoreWindow(m_WindowGLFW);
	}

	void WindowGLFW::SetFocused()
	{
		glfwFocusWindow(m_WindowGLFW);
	}

	void WindowGLFW::SetAttention()
	{
		glfwRequestWindowAttention(m_WindowGLFW);
	}

	void WindowGLFW::RegisterEvents()
	{
		// TODO glfwSetCharCallback

	// -----------------------------------------------------------------
	// ------------------------ MOUSE EVENTS ---------------------------
	// -----------------------------------------------------------------

		// MOUSE BUTTON PRESSED
		glfwSetMouseButtonCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, int buttonIndex, int actionType, int modifiers)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const MouseCode mouseCode = GLFWHelpers::GetMouseCodeFromGLFW(buttonIndex);
			const ModifierKeys modifierKeys = ModifierKeys(modifiers);
			switch (actionType)
			{
				case GLFW_PRESS:{
					const Events::Mouse::Press e(mouseCode, modifierKeys, window->GetWindowHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Press>(e);
					break;
				}
				case GLFW_RELEASE:{
					const Events::Mouse::Release e(mouseCode, modifierKeys, window->GetWindowHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Release>(e);
					break;
				}
				default:;
			}
		});
	
		
		// MOUSE ENTER/EXIT
		glfwSetCursorEnterCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, int isEntered)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			if(isEntered == GLFW_TRUE){
				const Events::Mouse::Enter e(window->GetWindowHandle());
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Enter>(e);
				#if defined DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS || defined DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
					ZERON_LOG_INFO("Window GLFW : Mouse cursor entered window '{}'", window->GetName())
				#endif
			}else{
				const Events::Mouse::Exit e(window->GetWindowHandle());
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Exit>(e);
				#if defined DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS || defined DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS 
					ZERON_LOG_INFO("Window GLFW : Mouse cursor exited window '{}'", window->GetName())
				#endif
			}
		});
		
		// MOUSE MOVED
		glfwSetCursorPosCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, double posX, double posY)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			window->m_WindowProps.MousePosition.X = static_cast<float>(posX);
			window->m_WindowProps.MousePosition.Y = static_cast<float>(posY);
			const Events::Mouse::Move e(static_cast<float>(posX), static_cast<float>(posY));
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Move>(e);
			#ifdef DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS
				ZERON_LOG_INFO("Window GLFW : Mouse moved X:{} | Y:{}", posX, posY)
			#endif
		});

		// MOUSE SCROLLED
		glfwSetScrollCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, double offsetX, double offsetY)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const Events::Mouse::Scroll e(static_cast<float>(offsetX), static_cast<float>(offsetY));
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse::Scroll>(e);
			#ifdef DEBUG_WINDOW_CONTEXT_MOUSE_EVENTS
				ZERON_LOG_INFO("Window GLFW : Mouse Scrolled {} {}", offsetX, offsetY)
			#endif
		});

	// -----------------------------------------------------------------
	// --------------------------- KEY EVENTS --------------------------
	// -----------------------------------------------------------------
		
		glfwSetKeyCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, int keyIndex, int scanCode, int actionType, int modifierMask)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const ModifierKeys modifierKeys = ModifierKeys(modifierMask);
			const KeyCode keyCode = KeyCode(GLFWHelpers::GetKeyCodeFromGLFW(keyIndex));
			switch (actionType)
			{
				case GLFW_PRESS:{
					const Events::Key::Press e(keyCode, modifierKeys, window->GetWindowHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Key::Press>(e);
					break;
				}
				case GLFW_RELEASE:{
					const Events::Key::Release e(keyCode, modifierKeys, window->GetWindowHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Key::Release>(e);
					break;
				}
				case GLFW_REPEAT:{
					const Events::Key::Repeat e(keyCode, modifierKeys, window->GetWindowHandle());
					window->m_WindowProps.EventDispatcher->Dispatch<Events::Key::Repeat>(e);
					break;
				}
				default:;
			}
		});

		glfwSetCharModsCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, unsigned int codepoint, int modifierMask)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const ModifierKeys modifierKeys = ModifierKeys(modifierMask);
			Events::Key::Character e(codepoint, modifierKeys, window->GetWindowHandle());
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
		glfwSetWindowSizeCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, int width, int height)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			window->m_WindowProps.HeightPrev = window->m_WindowProps.Height;
			window->m_WindowProps.WidthPrev = window->m_WindowProps.Width;
			window->m_WindowProps.Height = height;
			window->m_WindowProps.Width = width;
			const Events::Window::Resize e(width,height);
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Resize>(e);
			#ifdef DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
				ZERON_LOG_INFO("Window GLFW : Window '{}' resized to {}px-{}px", window->GetName(), width, width);
			#endif
		});
		
		// WINDOW CLOSED
		glfwSetWindowCloseCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const Events::Window::Close e(window->GetWindowHandle());
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Close>(e);
			#ifdef DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
				ZERON_LOG_INFO("Window GLFW : Window '{}' closed", window->GetName());
			#endif
		});

		// WINDOW FOCUSED/UNFOCUSED
		glfwSetWindowFocusCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, int isFocused)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			if(isFocused == GLFW_TRUE){
				const Events::Window::Focus e(window->GetWindowHandle());
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Focus>(e);
				#ifdef DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
					ZERON_LOG_INFO("Window GLFW : Window '{}' focused", window->GetName());
				#endif
			}else{
				const Events::Window::Unfocus e(window->GetWindowHandle());
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Unfocus>(e);
				#ifdef DEBUG_WINDOW_CONTEXT_WINDOW_EVENTS
					ZERON_LOG_INFO("Window GLFW : Window '{}' unfocused", window->GetName());
				#endif
			}
		});

		// WINDOW MINIMIZED/RESTORED
		glfwSetWindowIconifyCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, int isMinimized)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			if(isMinimized == GLFW_TRUE){
				const Events::Window::Minimize e;
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Minimize>(e);
			}else{
				const Events::Window::Restore e;
				window->m_WindowProps.EventDispatcher->Dispatch<Events::Window::Restore>(e);
			}
		});

		// WINDOW MAXIMIZED/RESTORED
		glfwSetWindowMaximizeCallback(m_WindowGLFW, [](GLFWwindow* windowGLFW, int isMaximized)
		{
			auto* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
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
