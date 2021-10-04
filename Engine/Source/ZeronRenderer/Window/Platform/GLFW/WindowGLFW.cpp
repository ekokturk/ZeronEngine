// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "WindowGLFW.h"

#if ZE_WINDOW_GLFW
#include <GLFW/glfw3.h>
#endif

namespace Zeron {

	int WindowGLFW::mWindowGLFWCount = 0;
	
	bool WindowGLFW::InitGLFW()
	{
	#if ZE_WINDOW_GLFW
		if(mWindowGLFWCount > 0) {
			return true;
		}

		if (glfwInit() == false) {
			// TODO: Assert
			return false;
		}

		int vMajor, vMinor, vPatch;
		glfwGetVersion(&vMajor, &vMinor, &vPatch);

		// Set error message callback for GLFW
		glfwSetErrorCallback([](int errorCode, const char* errorMessage) {
		});
		return true;
	#else
		return false;
	#endif
	}

	WindowGLFW::WindowGLFW(const WindowConfig& config)
		: Window(config)
		, mWindowGLFW(nullptr)
	{
		mWindowType = WindowType::GLFW;

		if(InitGLFW() == false) {
			return;
		}
	#if ZE_WINDOW_GLFW
		// Initialize window creation from window properties
		mMonitorGLFW = mIsFullScreen ? GetCurrentMonitor() : nullptr;
		const GLFWvidmode* videoMode = mIsFullScreen ? glfwGetVideoMode(mMonitorGLFW) : nullptr;
		const int initWidth = mIsFullScreen ? videoMode->width : mWidth;
		const int initHeight = mIsFullScreen ? videoMode->height : mHeight;

		// const int initRefreshRate = videoMode ? videoMode->refreshRate : mRefreshRate;
		//glfwWindowHint(GLFW_REFRESH_RATE, initRefreshRate);

		// Create Window
		mWindowGLFW = glfwCreateWindow(
			initWidth, 
			initHeight, 
			mName.c_str(), 
			mMonitorGLFW, 
			nullptr
		);
		
		if (mWindowGLFW == nullptr) {
			return;
		}

		mWindowGLFWCount++;
		glfwSetWindowUserPointer(mWindowGLFW, this);

		RegisterEvents();
	#endif
	}

	WindowGLFW::~WindowGLFW()
	{
	#if ZE_WINDOW_GLFW
		if(mWindowGLFW) {
			glfwDestroyWindow(mWindowGLFW);
			mWindowGLFWCount--;
		}

		if(mWindowGLFWCount == 0) {
			glfwTerminate();
		}
	#endif
	}

	void WindowGLFW::BeginFrame()
	{
	#if ZE_WINDOW_GLFW
		if (glfwWindowShouldClose(mWindowGLFW)) {
			return;
		}
	#endif
	}

	void WindowGLFW::EndFrame()
	{
	#if ZE_WINDOW_GLFW
		if (glfwWindowShouldClose(mWindowGLFW)) {
			return;
		}
		
		glfwPollEvents();
	#endif
	}


	void WindowGLFW::SetVisible()
	{
	#if ZE_WINDOW_GLFW
		glfwShowWindow(mWindowGLFW);
	#endif
	}

	void WindowGLFW::SetHidden()
	{
	#if ZE_WINDOW_GLFW
		glfwHideWindow(mWindowGLFW);
	#endif
	}
	
	void WindowGLFW::SetName(const std::string& name)
	{
	#if ZE_WINDOW_GLFW
		mName = name;
		glfwSetWindowTitle(mWindowGLFW, mName.c_str());
	#endif
	}

	void WindowGLFW::SetAspectRatio(int numerator, int denominator)
	{
	#if ZE_WINDOW_GLFW
		glfwSetWindowAspectRatio(mWindowGLFW, numerator, denominator);
	#endif
	}

	void WindowGLFW::SetSize(int width, int height)
	{
	#if ZE_WINDOW_GLFW
		mWidth = width;
		mHeight = height;
		glfwSetWindowSize(mWindowGLFW, width, height);
	#endif
	}

	void WindowGLFW::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight)
	{
	#if ZE_WINDOW_GLFW
		glfwSetWindowSizeLimits(mWindowGLFW, minWidth, minHeight, maxWidth, maxHeight);
	#endif
	}

	void WindowGLFW::SetScreenPosition(int posX, int posY)
	{
	#if ZE_WINDOW_GLFW
		// This callback will set the member parameters that will be triggered by this method
		glfwSetWindowPos(mWindowGLFW, posX, posY);
	#endif
	}

	void WindowGLFW::SetFullScreen(bool isFullScreen)
	{
	#if ZE_WINDOW_GLFW
		mIsFullScreen = isFullScreen;
		if(mIsFullScreen) {
			mMonitorGLFW = GetCurrentMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(mMonitorGLFW);
			glfwSetWindowMonitor(mWindowGLFW, mMonitorGLFW, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else {
			mMonitorGLFW = GetCurrentMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(mMonitorGLFW);
			glfwSetWindowMonitor(mWindowGLFW, nullptr, mScreenPrevPosX,
				mScreenPrevPosY, mWidthPrev, mHeightPrev, mode->refreshRate);
		}
	#endif
	}

	void* WindowGLFW::GetPlatformHandle()  const
	{
	#if ZE_WINDOW_GLFW
		return mWindowGLFW;
	#else
		return nullptr;
	#endif
	}

	GLFWmonitor* WindowGLFW::GetCurrentMonitor() const
	{
	#if ZE_WINDOW_GLFW
		// If window is already created, find the monitor of that window lives in
		if(mMonitorGLFW != nullptr)
		{
			
		}
		
		return glfwGetPrimaryMonitor();
	#else
		return nullptr;
	#endif
	}

	void WindowGLFW::SetMinimized()
	{
	#if ZE_WINDOW_GLFW
		glfwIconifyWindow(mWindowGLFW);
	#endif
	}

	void WindowGLFW::SetMaximized()
	{
	#if ZE_WINDOW_GLFW
		glfwMaximizeWindow(mWindowGLFW);
	#endif
	}

	void WindowGLFW::SetRestored()
	{
	#if ZE_WINDOW_GLFW
		glfwRestoreWindow(mWindowGLFW);
	#endif
	}

	void WindowGLFW::SetFocused()
	{
	#if ZE_WINDOW_GLFW
		glfwFocusWindow(mWindowGLFW);
	#endif
	}

	void WindowGLFW::SetAttention()
	{
	#if ZE_WINDOW_GLFW
		glfwRequestWindowAttention(mWindowGLFW);
	#endif
	}

	void WindowGLFW::RegisterEvents()
	{
	#if ZE_WINDOW_GLFW
	// ------------------------ MOUSE EVENTS ---------------------------
		// MOUSE BUTTON PRESSED
		glfwSetMouseButtonCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int buttonIndex, int actionType, int modifiers) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
			switch (actionType) {
				case GLFW_PRESS: {
				} break;
				case GLFW_RELEASE: {
				} break;
				default:;
			}
		});
		
		// MOUSE MOVED
		glfwSetCursorPosCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, double posX, double posY){
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
		});

		// MOUSE SCROLLED
		glfwSetScrollCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, double offsetX, double offsetY) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
		});

	// --------------------------- KEY EVENTS --------------------------
		glfwSetKeyCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int keyIndex, int scanCode, int actionType, int modifierMask) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
			switch (actionType) {
				case GLFW_PRESS:{
				} break;
				case GLFW_RELEASE:{
				} break;
				default:;
			}
		});

		glfwSetCharModsCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, unsigned int codepoint, int modifierMask) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
		});
		
	// ----------------------- GAMEPAD EVENTS --------------------------

		
	// ------------------------ WINDOW EVENTS --------------------------
		// WINDOW HOVER/UNHOVER
		glfwSetCursorEnterCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int isEntered) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
			switch (isEntered) {
				case GLFW_TRUE:	{
				} break;
				case GLFW_FALSE: {
				} break;
				default:;
			}
		});
		
		// WINDOW RESIZE
		glfwSetWindowSizeCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int width, int height) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);

		});
		
		// WINDOW CLOSED
		glfwSetWindowCloseCallback(mWindowGLFW, [](GLFWwindow* windowGLFW){
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
		});

		// WINDOW FOCUSED/UNFOCUSED
		glfwSetWindowFocusCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int isFocused) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
			switch (isFocused) {
				case GLFW_TRUE: {

				} break;
				case GLFW_FALSE: {

				} break;
				default:;
			}
		});

		// WINDOW MINIMIZED/RESTORED
		glfwSetWindowIconifyCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int isMinimized){
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
			switch (isMinimized) {
				case GLFW_TRUE: {

				} break;
				case GLFW_FALSE: {

				} break;
				default:;
			}
		});

		// WINDOW MAXIMIZED/RESTORED
		glfwSetWindowMaximizeCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int isMaximized) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);
			switch (isMaximized) {
				case GLFW_TRUE: {

				} break;
				case GLFW_FALSE: {

				} break;
				default:;
			}
		});

		glfwSetWindowPosCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int posX, int posY) {
			WindowGLFW* window = GetUserPointerGLFW(windowGLFW);

		});
	#endif
	}

	WindowGLFW* WindowGLFW::GetUserPointerGLFW(GLFWwindow* windowGLFW)
	{
	#if ZE_WINDOW_GLFW
		return static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
	#else 
		return nullptr;
	#endif
	}
}

