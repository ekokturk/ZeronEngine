// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window.h"

#include "Window/Platform/SDL/WindowSDL.h"
#include "Window/Platform/GLFW/WindowGLFW.h"
#include "Window/Platform/Win32/WindowWin32.h"

namespace Zeron {

	Window::Window(const WindowConfig& config)
		: mName(config.mName)
		, mID(config.mID)
		, mWidth(config.mWidth)
		, mHeight(config.mHeight)
		, mWidthPrev(mWidth)
		, mHeightPrev(mHeight)
		, mPosX(0)
		, mPosY(0)
		, mPosPrevX(mPosX)
		, mPosPrevY(mPosY)
		, mIsFullScreen(config.mIsFullScreen)
		, mIsMinimized(false)
		, mIsMaximized(false)
		, mIsHidden(false)
		, mWindowType(WindowAPI::Null)
		, mFullScreenType(FullScreenType::Borderless)
	{

	}

	std::unique_ptr<Window> Window::CreatePlatformWindow(WindowAPI type, const WindowConfig& config)
	{
		switch (type) {
		#if ZE_WINDOW_GLFW
			case WindowAPI::GLFW: {
				return std::make_unique<WindowGLFW>(config);
			} break;
		#endif
		#if ZE_WINDOW_SDL
			case WindowAPI::SDL: {
				return std::make_unique<WindowSDL>(config);
			} break;
		#endif
		#if ZE_WINDOW_WIN32
			case WindowAPI::Win32: {
				return std::make_unique<WindowWin32>(config);
			} break;
		#endif
			default: ZE_FAIL("Window API is not supported on this platform!");
		}
		return nullptr;
	}
	
	void Window::ClearEventQueue() 
	{
		std::queue<std::unique_ptr<WindowEvent>> empty;
		std::swap(mEventQueue, empty);
	}
	
	void Window::SetFullScreen(bool isFullScreen)
	{
		mIsFullScreen = isFullScreen;
		switch (mFullScreenType) {
		case Zeron::FullScreenType::Borderless: {
			OnFullScreenChangedBorderless();
		} break;
		case Zeron::FullScreenType::Monitor:
		default: ZE_FAIL("Active FullScreenType is not supported!");
		}
		SetClipCursor(mIsFullScreen);
	}

	void Window::SetFullScreenType(FullScreenType fullScreenType)
	{
		if(fullScreenType == mFullScreenType) {
			return;
		}
		// Disable the existing full screen config first
		const bool wasFullScreen = mIsFullScreen;
		if (wasFullScreen) {
			SetFullScreen(false);
		}
		mFullScreenType = fullScreenType;
		if (wasFullScreen) {
			SetFullScreen(true);
		}
	}

	std::unique_ptr<WindowEvent> Window::GetNextEvent()
	{
		if(!mEventQueue.empty()) {
			auto front = std::move(mEventQueue.front());
			mEventQueue.pop();
			return front;
		}
		return nullptr;
	}

	void Window::OnPositionChanged(int posX, int posY)
	{
		mPosPrevX = mPosX;
		mPosPrevY = mPosY;
		mPosX = posX;
		mPosY = posY;
	}

	void Window::OnHoverChanged(bool isHovered)
	{
		mIsHovered = isHovered;
	}

	void Window::OnFocusChanged(bool isFocused)
	{
		mIsFocused = isFocused;
	}

	void Window::OnVisibilityChanged(bool isHidden)
	{
		mIsHidden = isHidden;
	}

	void Window::OnRestored()
	{
		mIsMinimized = false;
		mIsMaximized = false;
	}

	void Window::OnMinimized()
	{
		mIsMinimized = true;
		mIsMaximized = false;
	}

	void Window::OnMaximized()
	{
		mIsMinimized = false;
		mIsMaximized = true;
	}

	void Window::OnSizeChanged(int width, int height)
	{
		mWidthPrev = mWidth;
		mHeightPrev = mHeight;
		mWidth = width;
		mHeight = height;
	}
}

