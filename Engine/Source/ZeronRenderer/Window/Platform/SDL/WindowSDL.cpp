// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "WindowSDL.h"

#if ZE_WINDOW_SDL
#include "SDL.h"
#include <SDL_syswm.h>
#endif

namespace Zeron {

	int WindowSDL::mWindowSDLCount = 0;

	WindowSDL::WindowSDL(const WindowConfig& config)
		: Window(config) 
		, mWindowSDL(nullptr)
	{
		mWindowType = WindowAPI::SDL;
	}
	
	WindowSDL::~WindowSDL()
	{
	#if ZE_WINDOW_SDL
		if(mWindowSDL) {
			SDL_DestroyWindow(mWindowSDL);
			mWindowSDLCount--;
			ZE_ASSERT(mWindowSDLCount >= 0, "Invalid SDL window count!");
		}

		if(mWindowSDLCount == 0) {
			SDL_Quit();
		}
	#endif
	}

	bool WindowSDL::Init()
	{
	#if ZE_WINDOW_SDL
		if (mWindowSDLCount == 0) {
			if (SDL_Init(SDL_INIT_VIDEO) != SDL_FALSE) {
				ZE_FAIL("SDL was not initialized!");
				return false;
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
			return false;
		}

		SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

		SDL_GetWindowPosition(mWindowSDL, &mPos.X, &mPos.Y);
		mPosPrev = mPos;
		
		mWindowSDLCount++;
		return true;
	#else
		ZE_FAIL("Current platform does not support SDL window!");
		return false;
	#endif

	}

	void WindowSDL::BeginFrame() {
		ProcessEvents();
	}

	void WindowSDL::EndFrame() {
		ClearEventQueue();
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
	#if ZE_WINDOW_SDL
		if(!IsFullScreen()) {
			ZE_FAIL("WindowSDL::SetAspectRatio Not implemented!");
		}
	#endif
	}

	void WindowSDL::SetSize(int width, int height)
	{
	#if ZE_WINDOW_SDL
		if (!IsFullScreen()) {
			SDL_SetWindowSize(mWindowSDL, width, height);
		}
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
		if (!IsFullScreen()) {
			// There is no event triggered for SDL_SetWindowPosition
			// So we update the cached position here
			OnPositionChanged(posX, posY);
			SDL_SetWindowPosition(mWindowSDL, mPos.X, mPos.Y);
		}
	#endif
	}

	void WindowSDL::SetClipCursor(bool shouldClip)
	{
#if ZE_WINDOW_SDL
		SDL_SetWindowGrab(mWindowSDL, mIsFullScreen ? SDL_TRUE : SDL_FALSE);
#endif
	}

	void WindowSDL::OnFullScreenChangedBorderless()
	{
	#if ZE_WINDOW_SDL
		SDL_SetWindowFullscreen(mWindowSDL, mIsFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	#endif
		}

	void WindowSDL::OnFullScreenChangedMonitor()
	{
	#if ZE_WINDOW_SDL
		SDL_SetWindowFullscreen(mWindowSDL, mIsFullScreen ? SDL_WINDOW_FULLSCREEN : 0);
	#endif
	}

	void* WindowSDL::GetAPIHandle() const
	{
	#if ZE_WINDOW_SDL
			return mWindowSDL;
	#else
			return nullptr;
	#endif
	}
	
	void* WindowSDL::GetPlatformHandle() const
	{
	#if ZE_PLATFORM_WIN32
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(mWindowSDL, &wmInfo);
		return wmInfo.info.win.window;
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
	#if ZE_WINDOW_SDL
		SDL_FlashWindow(mWindowSDL, SDL_FLASH_UNTIL_FOCUSED);
	#endif
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
				if(eventSDL.key.repeat == FALSE) {
					const KeyCode code = GetKeyCodeSDL(eventSDL.key.keysym.sym);
					mEventQueue.emplace(std::make_unique<WindowEvent_KeyDown>(code));
				}
			} break;
			case SDL_KEYUP: {
				if (eventSDL.key.repeat == FALSE) {
					const KeyCode code = GetKeyCodeSDL(eventSDL.key.keysym.sym);
					mEventQueue.emplace(std::make_unique<WindowEvent_KeyUp>(code));
				}
			} break;
			case SDL_TEXTINPUT: {
				mEventQueue.emplace(std::make_unique<WindowEvent_TextChar>(eventSDL.text.text[0]));
			} break;
			case SDL_MOUSEBUTTONDOWN: {
					const MouseCode code = GetMouseCodeSDL(eventSDL.button.button);
					mEventQueue.emplace(std::make_unique<WindowEvent_MouseDown>(code));
			} break;
			case SDL_MOUSEBUTTONUP: {
				const MouseCode code = GetMouseCodeSDL(eventSDL.button.button);
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseUp>(code));
			} break;
			case SDL_MOUSEMOTION: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseMoved>(eventSDL.button.x, eventSDL.button.y));
			} break;
			case SDL_MOUSEWHEEL: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseScrolled>(static_cast<float>(eventSDL.wheel.x),
					static_cast<float>(eventSDL.wheel.y)));
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
			OnVisibilityChanged(false);
		} break;
		case SDL_WINDOWEVENT_HIDDEN: {
			OnVisibilityChanged(true);
		} break;
		case SDL_WINDOWEVENT_EXPOSED: {
		} break;
		case SDL_WINDOWEVENT_MOVED: {
			OnPositionChanged(e.window.data1, e.window.data2);
			mEventQueue.emplace(std::make_unique<WindowEvent_WindowMoved>(e.window.data1, e.window.data2));
		} break;
		// This event is called after SDL_WINDOWEVENT_SIZE_CHANGED for external size change, user or window manager
		case SDL_WINDOWEVENT_RESIZED: {
		} break;
		case SDL_WINDOWEVENT_SIZE_CHANGED: {
			OnSizeChanged(e.window.data1, e.window.data2);
			mEventQueue.emplace(std::make_unique<WindowEvent_WindowResized>(e.window.data1, e.window.data2));
		} break;
		case SDL_WINDOWEVENT_MINIMIZED: {
			OnMinimized();
			mEventQueue.emplace(std::make_unique<WindowEvent_WindowMinimized>());
		} break;
		case SDL_WINDOWEVENT_MAXIMIZED: {
			OnMaximized();
			mEventQueue.emplace(std::make_unique<WindowEvent_WindowMaximized>());
		} break;
		case SDL_WINDOWEVENT_RESTORED: {
			OnRestored();
			mEventQueue.emplace(std::make_unique<WindowEvent_WindowRestored>());
		} break;
		case SDL_WINDOWEVENT_ENTER: {
			OnFocusChanged(true);
			mEventQueue.emplace(std::make_unique<WindowEvent_MouseEnter>());
		} break;
		case SDL_WINDOWEVENT_LEAVE: {
			OnFocusChanged(false);
			mEventQueue.emplace(std::make_unique<WindowEvent_MouseExit>());
		} break;
		case SDL_WINDOWEVENT_FOCUS_GAINED: {
			OnFocusChanged(true);
			mEventQueue.emplace(std::make_unique<WindowEvent_WindowFocused>());
		} break;
		case SDL_WINDOWEVENT_FOCUS_LOST: {
			OnFocusChanged(false);
			mEventQueue.emplace(std::make_unique<WindowEvent_WindowUnfocused>());
		} break;
		case SDL_WINDOWEVENT_CLOSE: {
			mEventQueue.emplace(std::make_unique<WindowEvent_WindowClosed>());
		} break;
		case SDL_WINDOWEVENT_TAKE_FOCUS: {
		} break;
		case SDL_WINDOWEVENT_HIT_TEST: {
		} break;
		default: break;
		}
	#endif
	}

	KeyCode WindowSDL::GetKeyCodeSDL(int32_t code) {
		switch (code) {
	#if ZE_WINDOW_SDL
		case SDLK_0:			return KeyCode::N0;
		case SDLK_1:			return KeyCode::N1;
		case SDLK_2:			return KeyCode::N2;
		case SDLK_3:			return KeyCode::N3;
		case SDLK_4:			return KeyCode::N4;
		case SDLK_5:			return KeyCode::N5;
		case SDLK_6:			return KeyCode::N6;
		case SDLK_7:			return KeyCode::N7;
		case SDLK_8:			return KeyCode::N8;
		case SDLK_9:			return KeyCode::N9;
		case SDLK_a:			return KeyCode::A;
		case SDLK_b:			return KeyCode::B;
		case SDLK_c:			return KeyCode::C;
		case SDLK_d:			return KeyCode::D;
		case SDLK_e:			return KeyCode::E;
		case SDLK_f:			return KeyCode::F;
		case SDLK_g:			return KeyCode::G;
		case SDLK_h:			return KeyCode::H;
		case SDLK_i:			return KeyCode::I;
		case SDLK_j:			return KeyCode::J;
		case SDLK_k:			return KeyCode::K;
		case SDLK_l:			return KeyCode::L;
		case SDLK_m:			return KeyCode::M;
		case SDLK_n:			return KeyCode::N;
		case SDLK_o:			return KeyCode::O;
		case SDLK_p:			return KeyCode::P;
		case SDLK_q:			return KeyCode::Q;
		case SDLK_r:			return KeyCode::R;
		case SDLK_s:			return KeyCode::S;
		case SDLK_t:			return KeyCode::T;
		case SDLK_u:			return KeyCode::U;
		case SDLK_v:			return KeyCode::V;
		case SDLK_w:			return KeyCode::W;
		case SDLK_x:			return KeyCode::X;
		case SDLK_y:			return KeyCode::Y;
		case SDLK_z:			return KeyCode::Z;
		case SDLK_ESCAPE:		return KeyCode::Escape;
		case SDLK_RETURN:		return KeyCode::Enter;
		case SDLK_TAB:			return KeyCode::Tab;
		case SDLK_BACKSPACE:	return KeyCode::Backspace;
		case SDLK_INSERT:		return KeyCode::Insert;
		case SDLK_DELETE:		return KeyCode::Delete;
		case SDLK_LSHIFT:		return KeyCode::LeftShift;
		case SDLK_LCTRL:		return KeyCode::LeftControl;
		case SDLK_LALT:			return KeyCode::LeftAlt;
		case SDLK_LGUI:			return KeyCode::LeftSuper;
		case SDLK_RSHIFT:		return KeyCode::RightShift;
		case SDLK_RCTRL:		return KeyCode::RightControl;
		case SDLK_RALT:			return KeyCode::RightAlt;
		case SDLK_RGUI:			return KeyCode::RightSuper;
		case SDLK_RIGHT:		return KeyCode::Right;
		case SDLK_LEFT:			return KeyCode::Left;
		case SDLK_DOWN:			return KeyCode::Down;
		case SDLK_UP:			return KeyCode::Up;
		case SDLK_PAGEUP:		return KeyCode::PageUp;
		case SDLK_PAGEDOWN:		return KeyCode::PageDown;
		case SDLK_HOME:			return KeyCode::Home;
		case SDLK_END:			return KeyCode::End;
		case SDLK_CAPSLOCK:		return KeyCode::CapsLock;
		case SDLK_SCROLLLOCK:	return KeyCode::ScrollLock;
		case SDLK_NUMLOCKCLEAR: return KeyCode::NumLock;
		case SDLK_PRINTSCREEN:	return KeyCode::PrintScreen;
		case SDLK_PAUSE:		return KeyCode::Pause;
		case SDLK_F1:			return KeyCode::F1;
		case SDLK_F2:			return KeyCode::F2;
		case SDLK_F3:			return KeyCode::F3;
		case SDLK_F4:			return KeyCode::F4;
		case SDLK_F5:			return KeyCode::F5;
		case SDLK_F6:			return KeyCode::F6;
		case SDLK_F7:			return KeyCode::F7;
		case SDLK_F8:			return KeyCode::F8;
		case SDLK_F9:			return KeyCode::F9;
		case SDLK_F10:			return KeyCode::F10;
		case SDLK_F11:			return KeyCode::F11;
		case SDLK_F12:			return KeyCode::F12;
		case SDLK_F13:			return KeyCode::F13;
		case SDLK_F14:			return KeyCode::F14;
		case SDLK_F15:			return KeyCode::F15;
		case SDLK_F16:			return KeyCode::F16;
		case SDLK_F17:			return KeyCode::F17;
		case SDLK_F18:			return KeyCode::F18;
		case SDLK_F19:			return KeyCode::F19;
		case SDLK_F20:			return KeyCode::F20;
		case SDLK_F21:			return KeyCode::F21;
		case SDLK_F22:			return KeyCode::F22;
		case SDLK_F23:			return KeyCode::F23;
		case SDLK_F24:			return KeyCode::F24;
		case SDLK_KP_0:			return KeyCode::KP0;
		case SDLK_KP_1:			return KeyCode::KP1;
		case SDLK_KP_2:			return KeyCode::KP2;
		case SDLK_KP_3:			return KeyCode::KP3;
		case SDLK_KP_4:			return KeyCode::KP4;
		case SDLK_KP_5:			return KeyCode::KP5;
		case SDLK_KP_6:			return KeyCode::KP6;
		case SDLK_KP_7:			return KeyCode::KP7;
		case SDLK_KP_8:			return KeyCode::KP8;
		case SDLK_KP_9:			return KeyCode::KP9;
		case SDLK_KP_PERIOD:	return KeyCode::KPDecimal;
		case SDLK_KP_DIVIDE:	return KeyCode::KPDivide;
		case SDLK_KP_MULTIPLY:	return KeyCode::KPMultiply;
		case SDLK_KP_MINUS:		return KeyCode::KPSubtract;
		case SDLK_KP_PLUS:		return KeyCode::KPAdd;
		case SDLK_KP_ENTER:		return KeyCode::KPEnter;
		case SDLK_KP_EQUALS:	return KeyCode::KPEqual;
		case SDLK_SPACE:		return KeyCode::Space;
		case SDLK_QUOTE:		return KeyCode::Apostrophe;
		case SDLK_COMMA:		return KeyCode::Comma;
		case SDLK_MINUS:		return KeyCode::Minus;
		case SDLK_PERIOD:		return KeyCode::Period;
		case SDLK_SLASH:		return KeyCode::Slash;
		case SDLK_SEMICOLON:	return KeyCode::Semicolon;
		case SDLK_EQUALS:		return KeyCode::Equal;
		case SDLK_LEFTBRACKET:	return KeyCode::LeftBracket;
		case SDLK_RIGHTBRACKET: return KeyCode::RightBracket;
		case SDLK_BACKSLASH:	return KeyCode::Backslash;
		case SDLK_BACKQUOTE:	return KeyCode::Tilde;
		case SDLK_MENU:			return KeyCode::Menu;
		#endif
		default:				return KeyCode::Unknown;
		}
	}

	MouseCode WindowSDL::GetMouseCodeSDL(int32_t code)
	{
		switch (code) {
		#if ZE_WINDOW_SDL
		case SDL_BUTTON_LEFT:	return MouseCode::LeftButton;
		case SDL_BUTTON_RIGHT:	return MouseCode::RightButton;
		case SDL_BUTTON_MIDDLE: return MouseCode::MiddleButton;
		case SDL_BUTTON_X1:		return MouseCode::ButtonX1;
		case SDL_BUTTON_X2:		return MouseCode::ButtonX2;
		#endif
		default:				return MouseCode::Unknown;
		}
	}

}
