// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Window/Window.h>

#include <Window/API/GLFW/WindowGLFW.h>
#include <Window/API/SDL/WindowSDL.h>
#include <Window/API/Win32/WindowWin32.h>

namespace Zeron
{
	Window::Window(const WindowConfig& config, WindowAPI api)
		: mName(config.mName)
		, mID(config.mID)
		, mSize{ config.mWidth, config.mHeight }
		, mSizePrev(mSize)
		, mPos {0,0}
		, mPosPrev{mPos.X, mPos.Y}
		, mIsFullScreen(config.mIsFullScreen)
		, mIsMinimized(false)
		, mIsMaximized(false)
		, mIsHidden(false)
		, mFullScreenType(FullScreenType::Borderless)
		, mWindowType(api)
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

	Vec2i Window::GetCenter() const 
	{
		return { mSize.X / 2, mSize.Y / 2 };
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
			_onFullScreenChangedBorderless();
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
		mPosPrev = mPos;
		mPos = { posX, posY };
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
		mSizePrev = mSize;
		mSize = { width, height };
	}
}

