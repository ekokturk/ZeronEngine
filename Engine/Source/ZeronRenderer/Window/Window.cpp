// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window.h"

#include "Window/Platform/SDL/WindowSDL.h"
#include "Window/Platform/GLFW/WindowGLFW.h"

namespace Zeron {

	Window::Window(const WindowConfig& config)
		: mName(config.mName)
		, mID(config.mID)
		, mWidth(config.mWidth)
		, mHeight(config.mHeight)
		, mWidthPrev(mWidth)
		, mHeightPrev(mHeight)
		, mScreenPosX(0)
		, mScreenPosY(0)
		, mScreenPrevPosX(mScreenPosX)
		, mScreenPrevPosY(mScreenPosY)
		, mIsFullScreen(config.mIsFullScreen)
		, mIsMinimized(false)
		, mWindowType(WindowType::Null)
	{

	}

	std::unique_ptr<Window> Window::CreateWindow(WindowType type, const WindowConfig& config)
	{
		switch (type) {
			case WindowType::GLFW: {
			#if ZE_WINDOW_GLFW
				return std::make_unique<WindowGLFW>(config);
			#endif
			} break;
			case WindowType::SDL: {
			#if ZE_WINDOW_SDL
					return std::make_unique<WindowSDL>(config);
			#endif
			} break;
			case WindowType::Win32: {
			#if ZE_WINDOW_WIN32
			#endif
			} break;
			default:
				// TODO: Assert here
				break;
		}
		return nullptr;
	}
}

