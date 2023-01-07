// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_GLFW
#include <Platform/API/Shared/GLFW/WindowGLFW.h>

#include <Platform/API/Shared/GLFW/GLFWHelpers.h>
#include <GLFW/glfw3.h>

namespace Zeron {

	int WindowGLFW::mWindowGLFWCount = 0;

	WindowGLFW::WindowGLFW(const WindowConfig& config, void* userData)
		: Window(config, WindowAPI::GLFW)
		, mWindowGLFW(nullptr)
		, mMonitorGLFW(nullptr)
		, mCursorGLFW(nullptr)
		, mIsCursorClipped(false)
	{
		glfwSetErrorCallback([](int errorCode, const char* errorMessage) {
			ZE_LOGE("GLFW ERROR {}: {}", errorCode, errorMessage);
		});

		if (mWindowGLFWCount == 0) {
			if (glfwInit() == GLFW_FALSE) {
				ZE_FAIL("GLFW was not initialized!");
			}
			int vMajor, vMinor, vPatch;
			glfwGetVersion(&vMajor, &vMinor, &vPatch);
			ZE_LOGI("GLFW v{}.{}.{} initialized...", vMajor, vMinor, vPatch);
		}

		// const int initRefreshRate = videoMode ? videoMode->refreshRate : mRefreshRate;
		//glfwWindowHint(GLFW_REFRESH_RATE, initRefreshRate);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		mWindowGLFW = glfwCreateWindow(
			mSize.X,
			mSize.Y,
			mName.c_str(),
			mMonitorGLFW,
			nullptr
		);

		if (!mWindowGLFW) {
			ZE_FAIL("GLFW window was not created!");
		}

		glfwSetWindowUserPointer(mWindowGLFW, userData);

		// We need to initialize cached postion
		glfwGetWindowPos(mWindowGLFW, &mPos.X, &mPos.Y);
		mPosPrev = mPos;

		mWindowGLFWCount++;
	}

	WindowGLFW::~WindowGLFW()
	{
		if(mWindowGLFW) {
			glfwDestroyWindow(mWindowGLFW);
			mWindowGLFWCount--;
			ZE_ASSERT(mWindowGLFWCount >= 0, "Invalid SDL window count!");
		}

		if(mWindowGLFWCount == 0) {
			glfwTerminate();
		}
	}

	void WindowGLFW::Update()
	{
		//ClearEventQueue();
		if (glfwWindowShouldClose(mWindowGLFW)) {
			return;
		}
		if (mIsCursorClipped) {
			// OPTIMIZE: This is not ideal, find a better way
			double mouseX, mouseY;
			glfwGetCursorPos(mWindowGLFW, &mouseX, &mouseY);
			const int newMouseX = std::clamp<int>(static_cast<int>(mouseX), mPos.X, mPos.X + mSize.X);
			const int newMouseY = std::clamp<int>(static_cast<int>(mouseY), mPos.Y, mPos.Y + mSize.Y);
			if (newMouseX != static_cast<int>(mouseX) || newMouseY != static_cast<int>(mouseY)) {
				glfwSetCursorPos(mWindowGLFW, newMouseX, newMouseY);
			}
		}

		// Queue blocked until resizing is over so we send the final size with the event
		if(mIsResizing) {
			OnSystemEvent({ SystemEvent::WindowResized{ mSize.X, mSize.Y } } );
			mIsResizing = false;
		}
	}

	void WindowGLFW::SetVisible()
	{
		glfwShowWindow(mWindowGLFW);
		mIsHidden = false;
	}

	void WindowGLFW::SetHidden()
	{
		glfwHideWindow(mWindowGLFW);
		mIsHidden = true;
	}
	
	void WindowGLFW::SetName(const std::string& name)
	{
		mName = name;
		glfwSetWindowTitle(mWindowGLFW, mName.c_str());
	}

	void WindowGLFW::SetAspectRatio(int numerator, int denominator)
	{
		if (!IsFullScreen()) {
			glfwSetWindowAspectRatio(mWindowGLFW, numerator, denominator);
		}
	}

	void WindowGLFW::SetSize(int width, int height)
	{
		if (!IsFullScreen()) {
			glfwSetWindowSize(mWindowGLFW, width, height);
		}
	}

	void WindowGLFW::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight)
	{
		glfwSetWindowSizeLimits(mWindowGLFW, minWidth, minHeight, maxWidth, maxHeight);
	}

	void WindowGLFW::SetScreenPosition(int posX, int posY)
	{
		if (!IsFullScreen()) {
			glfwSetWindowPos(mWindowGLFW, posX, posY);
		}
	}

	void WindowGLFW::SetClipCursor(bool shouldClip)
	{
		mIsCursorClipped = shouldClip;
	}

	void WindowGLFW::_onFullScreenChangedBorderless()
	{
		if (mIsFullScreen) {
			GLFWmonitor* monitor = FindCurrentMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			const int width = mode->width;
			const int height = mode->height;
			int xPos, yPos;
			glfwGetMonitorPos(monitor, &xPos, &yPos);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_FALSE);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_TRUE);
			glfwSetWindowMonitor(mWindowGLFW, nullptr, xPos, yPos, width, height, mode->refreshRate);
		}
		else {
			GLFWmonitor* monitor = FindCurrentMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_FALSE);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_TRUE);
			glfwSetWindowMonitor(mWindowGLFW, nullptr, mPosPrev.X, mPosPrev.Y, mSizePrev.X, mSizePrev.Y, mode->refreshRate);
		}
	}

	void WindowGLFW::_onFullScreenChangedMonitor()
	{
		if(mIsFullScreen) {
			GLFWmonitor* monitor = FindCurrentMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_FALSE);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_TRUE);
			glfwSetWindowMonitor(mWindowGLFW, monitor, 0, 0, mSize.X, mSize.Y, mode->refreshRate);
		}
		else {
			GLFWmonitor* monitor = FindCurrentMonitor();
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_TRUE);
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(mWindowGLFW, nullptr, mPosPrev.X, mPosPrev.Y, mSizePrev.X, mSizePrev.Y, mode->refreshRate);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_FALSE);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowAttrib(mWindowGLFW, GLFW_RESIZABLE, GLFW_TRUE);
		}
	}

	void* WindowGLFW::GetApiHandle() const
	{
		return mWindowGLFW;
	}
	
	SystemHandle WindowGLFW::GetSystemHandle()  const
	{
		return GLFWHelpers::GetPlatformWindowHandle(mWindowGLFW);
	}

	void WindowGLFW::SetMinimized()
	{
		glfwIconifyWindow(mWindowGLFW);
	}

	void WindowGLFW::SetMaximized()
	{
		glfwMaximizeWindow(mWindowGLFW);
	}

	void WindowGLFW::SetRestored()
	{
		glfwRestoreWindow(mWindowGLFW);
	}

	void WindowGLFW::SetFocused()
	{
		glfwFocusWindow(mWindowGLFW);
	}

	void WindowGLFW::SetAttention()
	{
		glfwRequestWindowAttention(mWindowGLFW);
	}

	GLFWmonitor* WindowGLFW::FindCurrentMonitor() const
	{
		GLFWmonitor* result;
		long areaSize = 0;
		int monitorCount = 0;
		GLFWmonitor** monitorList = glfwGetMonitors(&monitorCount);
		for (int i = 0; i < monitorCount; i++) {
			GLFWmonitor* monitor = monitorList[i];
			int xPos, yPos;
			glfwGetMonitorPos(monitor, &xPos, &yPos);
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			const int width = mode->width;
			const int height = mode->height;

			// Get the monitor with the largest intersection area
			const int aX = std::max<int>(xPos, mPos.X);
			const int aY = std::max<int>(yPos, mPos.Y);
			const int bX = std::min<int>(xPos + width, mPos.X + mSize.X);
			const int bY = std::min<int>(yPos + height, mPos.Y + mSize.Y);
			if (aX > bX || aY > bY) {
				continue;
			}
			const long calcArea = std::abs((bY - aY) * (bX - aX));
			if (calcArea > areaSize) {
				result = monitor;
			}
		}
		return result;
	}
}

#endif