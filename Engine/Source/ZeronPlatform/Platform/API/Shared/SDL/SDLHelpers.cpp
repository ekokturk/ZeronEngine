// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_SDL
#include <Platform/API/Shared/SDL/SDLHelpers.h>

#if ZE_PLATFORM_LINUX
    #define SDL_VIDEO_DRIVER_X11
#endif
#include <SDL.h>
#include <SDL_syswm.h>

namespace Zeron {

    SystemHandle SDLHelpers::GetPlatformWindowHandle(SDL_Window* sdlWindow) {
        SystemHandle handle;
#if ZE_PLATFORM_WIN32
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
        handle.mWindow = wmInfo.info.win.window;
#elif ZE_PLATFORM_LINUX
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
        handle.mWindow = wmInfo.info.x11.window;		
        handle.mConnection = wmInfo.info.x11.display;		
#elif ZE_PLATFORM_ANDROID
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
        handle.mWindow = wmInfo.info.android.window;		
#endif
        return handle;
    }

    KeyCode SDLHelpers::GetKeyCode(int32_t code)
    {
		switch (code) {
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
		default:				return KeyCode::Unknown;
		}
    }

    MouseCode SDLHelpers::GetMouseCode(int32_t code)
    {
        switch (code) {
        case SDL_BUTTON_LEFT:	return MouseCode::LeftButton;
        case SDL_BUTTON_RIGHT:	return MouseCode::RightButton;
        case SDL_BUTTON_MIDDLE: return MouseCode::MiddleButton;
        case SDL_BUTTON_X1:		return MouseCode::ButtonX1;
        case SDL_BUTTON_X2:		return MouseCode::ButtonX2;
        default:				return MouseCode::Unknown;
        }
    }
}

#endif