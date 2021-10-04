// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "WindowSDL.h"

#if ZE_WINDOW_SDL
#include "SDL.h"
#include <SDL_syswm.h>
#endif

namespace Zeron {

	int WindowSDL::mWindowSDLCount = 0;

	bool WindowSDL::InitSDL()
	{
	#if ZE_WINDOW_SDL
		if (mWindowSDLCount > 0) {
			// SDL is already initialized
			return true;
		}

		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			return false;
		}
		SDL_version versionSDL;
		SDL_GetVersion(&versionSDL);

		// TODO: Initialize SDL2 Error Callback

		return true;
	#else
		return false;
	#endif

	}

	WindowSDL::WindowSDL(const WindowConfig& config)
		: Window(config) 
		, mWindowSDL(nullptr)
	{
		mWindowType = WindowType::SDL;

		if (InitSDL() == false) {
			// TODO: Assert
			return;
		}


	#if ZE_WINDOW_SDL
		SDL_DisplayMode displayMode;
		const int currentDisplay = GetCurrentDisplay();
		SDL_GetCurrentDisplayMode(currentDisplay, &displayMode);
		const int initWidth = mIsFullScreen ? displayMode.w : mWidth;
		const int initHeight = mIsFullScreen ? displayMode.h : mHeight;
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

		if (mWindowSDL == nullptr) {
			//	TODO: Assert;
			return;
		}

		mWindowSDLCount++;
	#endif

	}
	
	WindowSDL::~WindowSDL()
	{
	#if ZE_WINDOW_SDL
		if(mWindowSDL) {
			SDL_DestroyWindow(mWindowSDL);
			mWindowSDLCount--;
		}

		if(mWindowSDLCount == 0) {
			SDL_Quit();
		}
	#endif
	}

	void WindowSDL::BeginFrame() {
		ProcessEvents();
	}

	void WindowSDL::EndFrame() {
	}

	void WindowSDL::SetVisible()
	{
	#if ZE_WINDOW_SDL
		SDL_ShowWindow(mWindowSDL);
	#endif
	}

	void WindowSDL::SetHidden()
	{
	#if ZE_WINDOW_SDL
		SDL_HideWindow(mWindowSDL);
	#endif
	}

	void WindowSDL::SetName(const std::string& name)
	{
	#if ZE_WINDOW_SDL
		mName = name;
		SDL_SetWindowTitle(mWindowSDL, mName.c_str());
	#endif
	}

	void WindowSDL::SetAspectRatio(int numerator, int denominator)
	{
	}

	void WindowSDL::SetSize(int width, int height)
	{
	#if ZE_WINDOW_SDL
		mWidth = width;
		mHeight = height;
		SDL_SetWindowSize(mWindowSDL, width, height);
	#endif
	}

	void WindowSDL::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight)
	{
	#if ZE_WINDOW_SDL
		SDL_SetWindowMaximumSize(mWindowSDL, maxWidth, maxHeight);
		SDL_SetWindowMinimumSize(mWindowSDL, minWidth, minHeight);
	#endif
	}

	void WindowSDL::SetScreenPosition(int posX, int posY)
	{
	#if ZE_WINDOW_SDL
		SDL_SetWindowPosition(mWindowSDL, posX, posY);
	#endif
	}

	void WindowSDL::SetFullScreen(bool isFullScreen)
	{
	#if ZE_WINDOW_SDL
		mIsFullScreen = isFullScreen;
		SDL_SetWindowFullscreen(mWindowSDL, isFullScreen ? SDL_WINDOW_FULLSCREEN : 0);
	#endif
	}

	void* WindowSDL::GetPlatformHandle() const
	{
	#if ZE_WINDOW_SDL
		return mWindowSDL;
	#else
		return nullptr;
	#endif
	}

	int WindowSDL::GetCurrentDisplay() const
	{
	#if ZE_WINDOW_SDL
		return mWindowSDL ? SDL_GetWindowDisplayIndex(mWindowSDL) : 1;
	#else
		return -1;
	#endif
	}

	void WindowSDL::SetMinimized()
	{
	#if ZE_WINDOW_SDL
		SDL_MinimizeWindow(mWindowSDL);
	#endif
	}

	void WindowSDL::SetMaximized()
	{
	#if ZE_WINDOW_SDL
		SDL_MaximizeWindow(mWindowSDL);
	#endif
	}

	void WindowSDL::SetRestored()
	{
	#if ZE_WINDOW_SDL
		SDL_RestoreWindow(mWindowSDL);
	#endif
	}

	void WindowSDL::SetFocused()
	{
	#if ZE_WINDOW_SDL
		SDL_SetWindowInputFocus(mWindowSDL);
	#endif
	}

	void WindowSDL::SetAttention()
	{
		
	}

	void WindowSDL::ProcessEvents()
	{
	#if ZE_WINDOW_SDL
		SDL_Event eventSDL;
		while(SDL_PollEvent(&eventSDL)) {
			switch (eventSDL.type) {
			case SDL_QUIT: {
			} break;
			case SDL_APP_TERMINATING: {
			} break;
			case SDL_APP_LOWMEMORY: {
			} break;
			case SDL_APP_WILLENTERBACKGROUND: {
			} break;
			case SDL_APP_DIDENTERBACKGROUND: {
			} break;
			case SDL_APP_WILLENTERFOREGROUND: {
			} break;
			case SDL_APP_DIDENTERFOREGROUND: {
			} break;
			case SDL_WINDOWEVENT: {
				ProcessWindowEvents(eventSDL);
			} break;
			case SDL_KEYDOWN: {
			} break;
			case SDL_KEYUP: {
			} break;
			default:;
			}
		}
	#endif
	}

	void WindowSDL::ProcessWindowEvents(const SDL_Event& e)
	{
	#if ZE_WINDOW_SDL

		const Uint32 windowID = SDL_GetWindowID(mWindowSDL);
		if(e.window.windowID != windowID) {
			return;
		}
		
		switch (e.window.event) {
		case SDL_WINDOWEVENT_SHOWN: {
			//SDL_Log("Window %d shown", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_HIDDEN: {
			//SDL_Log("Window %d hidden", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_EXPOSED: {
			//SDL_Log("Window %d exposed", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_MOVED: {
			//SDL_Log("Window %d moved to %d,%d", e.window.windowID, e.window.data1,e.window.data2);
		} break;
		case SDL_WINDOWEVENT_RESIZED: {
			//SDL_Log("Window %d resized to %dx%d", e.window.windowID, e.window.data1, e.window.data2);
		} break;
		case SDL_WINDOWEVENT_SIZE_CHANGED: {
			//SDL_Log("Window %d size changed to %dx%d", e.window.windowID, e.window.data1, e.window.data2);
		} break;
		case SDL_WINDOWEVENT_MINIMIZED: {
			//SDL_Log("Window %d minimized", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_MAXIMIZED: {
			//SDL_Log("Window %d maximized", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_RESTORED: {
			//SDL_Log("Window %d restored", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_ENTER: {
		} break;
		case SDL_WINDOWEVENT_LEAVE: {
		} break;
		case SDL_WINDOWEVENT_FOCUS_GAINED: {
			//SDL_Log("Window %d gained keyboard focus", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_FOCUS_LOST: {
			//SDL_Log("Window %d lost keyboard focus", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_CLOSE: {
			//SDL_Log("Window %d closed", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_TAKE_FOCUS: {
			//SDL_Log("Window %d is offered a focus", e.window.windowID);
		} break;
		case SDL_WINDOWEVENT_HIT_TEST: {
			//SDL_Log("Window %d has a special hit test", e.window.windowID);
		} break;
		default: break;
			//SDL_Log("Window %d got unknown event %d", e.window.windowID, e.window.event);
		}
	#endif
	}

	
}
