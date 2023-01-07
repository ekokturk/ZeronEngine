// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_SDL

#include <Platform/API/Shared/SDL/WindowSDL.h>

#include <Platform/API/Shared/SDL/SDLHelpers.h>
#include <SDL.h>
#include <SDL_syswm.h>

namespace Zeron {

	int WindowSDL::mWindowSDLCount = 0;

	WindowSDL::WindowSDL(const WindowConfig& config)
		: Window(config, WindowAPI::SDL)
		, mWindowSDL(nullptr)
	{
		if (mWindowSDLCount == 0) {
			if (SDL_Init(SDL_INIT_VIDEO) != SDL_FALSE) {
				ZE_FAIL("SDL was not initialized!");
			}
			SDL_version versionSDL;
			SDL_GetVersion(&versionSDL);
			ZE_LOGI("SDL v{}.{}.{} initialized...", versionSDL.major, versionSDL.minor, versionSDL.patch);
		}

		SDL_DisplayMode displayMode;
		const int currentDisplay = GetCurrentDisplay();
		SDL_GetCurrentDisplayMode(currentDisplay, &displayMode);

		const int initWidth = mIsFullScreen ? displayMode.w : mSize.X;
		const int initHeight = mIsFullScreen ? displayMode.h : mSize.Y;
		// TODO: Support refresh rate
		const int initRefreshRate = displayMode.refresh_rate;
		const SDL_WindowFlags initFlags = static_cast<SDL_WindowFlags>(
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
		);

		// Create Window
		mWindowSDL = SDL_CreateWindow(
			mName.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			initWidth,
			initHeight,
			initFlags
		);

		if (!mWindowSDL) {
			ZE_FAIL("SDL window was not created!");
		}

		SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

		SDL_GetWindowPosition(mWindowSDL, &mPos.X, &mPos.Y);
		mPosPrev = mPos;

		mWindowSDLCount++;
	}
	
	WindowSDL::~WindowSDL()
	{
		if(mWindowSDL) {
			SDL_DestroyWindow(mWindowSDL);
			mWindowSDLCount--;
			ZE_ASSERT(mWindowSDLCount >= 0, "Invalid SDL window count!");
		}

		if(mWindowSDLCount == 0) {
			SDL_Quit();
		}
	}

	void WindowSDL::SetVisible()
	{
		SDL_ShowWindow(mWindowSDL);
	}

	void WindowSDL::SetHidden()
	{
		SDL_HideWindow(mWindowSDL);
	}

	void WindowSDL::SetName(const std::string& name)
	{
		mName = name;
		SDL_SetWindowTitle(mWindowSDL, mName.c_str());
	}

	void WindowSDL::SetAspectRatio(int numerator, int denominator)
	{
		if(!IsFullScreen()) {
			ZE_FAIL("WindowSDL::SetAspectRatio Not implemented!");
		}
	}

	void WindowSDL::SetSize(int width, int height)
	{
		if (!IsFullScreen()) {
			SDL_SetWindowSize(mWindowSDL, width, height);
		}
	}

	void WindowSDL::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight)
	{
		SDL_SetWindowMaximumSize(mWindowSDL, maxWidth, maxHeight);
		SDL_SetWindowMinimumSize(mWindowSDL, minWidth, minHeight);
	}

	void WindowSDL::SetScreenPosition(int posX, int posY)
	{
		if (!IsFullScreen()) {
			// There is no event triggered for SDL_SetWindowPosition
			// So we update the cached position here
			OnPositionChanged(posX, posY);
			SDL_SetWindowPosition(mWindowSDL, mPos.X, mPos.Y);
		}
	}

	void WindowSDL::SetClipCursor(bool shouldClip)
	{
		SDL_SetWindowGrab(mWindowSDL, mIsFullScreen ? SDL_TRUE : SDL_FALSE);
	}

	void WindowSDL::_onFullScreenChangedBorderless()
	{
		SDL_SetWindowFullscreen(mWindowSDL, mIsFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}

	void WindowSDL::_onFullScreenChangedMonitor()
	{
		SDL_SetWindowFullscreen(mWindowSDL, mIsFullScreen ? SDL_WINDOW_FULLSCREEN : 0);
	}

	void* WindowSDL::GetApiHandle() const
	{
		return mWindowSDL;
	}
	
	SystemHandle WindowSDL::GetSystemHandle() const
	{
		return SDLHelpers::GetPlatformWindowHandle(mWindowSDL);
	}

	int WindowSDL::GetCurrentDisplay() const
	{
		return mWindowSDL ? SDL_GetWindowDisplayIndex(mWindowSDL) : 1;
	}

	void WindowSDL::SetMinimized()
	{
		SDL_MinimizeWindow(mWindowSDL);
	}

	void WindowSDL::SetMaximized()
	{
		SDL_MaximizeWindow(mWindowSDL);
	}

	void WindowSDL::SetRestored()
	{
		SDL_RestoreWindow(mWindowSDL);
	}

	void WindowSDL::SetFocused()
	{
		SDL_SetWindowInputFocus(mWindowSDL);
	}

	void WindowSDL::SetAttention()
	{
		SDL_FlashWindow(mWindowSDL, SDL_FLASH_UNTIL_FOCUSED);
	}
}

#endif
