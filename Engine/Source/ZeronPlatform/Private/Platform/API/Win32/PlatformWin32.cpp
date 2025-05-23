// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_WIN32

#	include <Platform/API/Win32/PlatformWin32.h>
#	include <Core/FileSystem.h>

#	include <Platform/WindowManager.h>

#	define GETSCANCODE(lParam)	 (lParam & 0x00ff0000) >> 16
#	define EXTENDED(lParam)	 (lParam & 0x01000000) != 0
#	define NOTREPEATING(lParam) (lParam & 0x40000000) == 0

namespace Zeron
{

	PlatformWin32::PlatformWin32(const PlatformCreationProps&) {}

	PlatformWin32::~PlatformWin32() {}

	bool PlatformWin32::Init()
	{
		const bool result = Platform::Init();
		return result;
	}

	Window* PlatformWin32::CreatePlatformWindow(WindowConfig&& config)
	{
		if (!config.HasValidIcon()) {
			// Load default engine icon
			config.mIcon = _readWindowIcon();
		}
		return mWindowManager->RegisterWindow(std::move(config), WindowAPI::Win32);
	}

	KeyCode PlatformWin32::GetKeyCode(WPARAM wParam, LPARAM lParam)
	{
		switch (wParam) {
			case 0x30: return KeyCode::N0;
			case 0x31: return KeyCode::N1;
			case 0x32: return KeyCode::N2;
			case 0x33: return KeyCode::N3;
			case 0x34: return KeyCode::N4;
			case 0x35: return KeyCode::N5;
			case 0x36: return KeyCode::N6;
			case 0x37: return KeyCode::N7;
			case 0x38: return KeyCode::N8;
			case 0x39: return KeyCode::N9;
			case 0x41: return KeyCode::A;
			case 0x42: return KeyCode::B;
			case 0x43: return KeyCode::C;
			case 0x44: return KeyCode::D;
			case 0x45: return KeyCode::E;
			case 0x46: return KeyCode::F;
			case 0x47: return KeyCode::G;
			case 0x48: return KeyCode::H;
			case 0x49: return KeyCode::I;
			case 0x4A: return KeyCode::J;
			case 0x4B: return KeyCode::K;
			case 0x4C: return KeyCode::L;
			case 0x4D: return KeyCode::M;
			case 0x4E: return KeyCode::N;
			case 0x4F: return KeyCode::O;
			case 0x50: return KeyCode::P;
			case 0x51: return KeyCode::Q;
			case 0x52: return KeyCode::R;
			case 0x53: return KeyCode::S;
			case 0x54: return KeyCode::T;
			case 0x55: return KeyCode::U;
			case 0x56: return KeyCode::V;
			case 0x57: return KeyCode::W;
			case 0x58: return KeyCode::X;
			case 0x59: return KeyCode::Y;
			case 0x5A: return KeyCode::Z;
			case VK_ESCAPE: return KeyCode::Escape;
			case VK_RETURN: return KeyCode::Enter;
			case VK_TAB: return KeyCode::Tab;
			case VK_BACK: return KeyCode::Backspace;
			case VK_INSERT: return KeyCode::Insert;
			case VK_DELETE: return KeyCode::Delete;
			case VK_SHIFT: {
				switch (MapVirtualKey(GETSCANCODE(lParam), MAPVK_VSC_TO_VK_EX)) {
					case VK_LSHIFT: return KeyCode::LeftShift;
					case VK_RSHIFT: return KeyCode::RightShift;
				}
			} break;
			case VK_CONTROL: return EXTENDED(lParam) ? KeyCode::RightControl : KeyCode::LeftControl;
			case VK_MENU: return EXTENDED(lParam) ? KeyCode::RightAlt : KeyCode::LeftAlt;
			case VK_LWIN: return KeyCode::LeftSuper;
			case VK_RWIN: return KeyCode::RightSuper;
			case VK_RIGHT: return KeyCode::Right;
			case VK_LEFT: return KeyCode::Left;
			case VK_DOWN: return KeyCode::Down;
			case VK_UP: return KeyCode::Up;
			case VK_PRIOR: return KeyCode::PageUp;
			case VK_NEXT: return KeyCode::PageDown;
			case VK_HOME: return KeyCode::Home;
			case VK_END: return KeyCode::End;
			case VK_CAPITAL: return KeyCode::CapsLock;
			case VK_SCROLL: return KeyCode::ScrollLock;
			case VK_NUMLOCK: return KeyCode::NumLock;
			case VK_SNAPSHOT: return KeyCode::PrintScreen;
			case VK_PAUSE: return KeyCode::Pause;
			case VK_F1: return KeyCode::F1;
			case VK_F2: return KeyCode::F2;
			case VK_F3: return KeyCode::F3;
			case VK_F4: return KeyCode::F4;
			case VK_F5: return KeyCode::F5;
			case VK_F6: return KeyCode::F6;
			case VK_F7: return KeyCode::F7;
			case VK_F8: return KeyCode::F8;
			case VK_F9: return KeyCode::F9;
			case VK_F10: return KeyCode::F10;
			case VK_F11: return KeyCode::F11;
			case VK_F12: return KeyCode::F12;
			case VK_F13: return KeyCode::F13;
			case VK_F14: return KeyCode::F14;
			case VK_F15: return KeyCode::F15;
			case VK_F16: return KeyCode::F16;
			case VK_F17: return KeyCode::F17;
			case VK_F18: return KeyCode::F18;
			case VK_F19: return KeyCode::F19;
			case VK_F20: return KeyCode::F20;
			case VK_F21: return KeyCode::F21;
			case VK_F22: return KeyCode::F22;
			case VK_F23: return KeyCode::F23;
			case VK_F24: return KeyCode::F24;
			case VK_NUMPAD0: return KeyCode::KP0;
			case VK_NUMPAD1: return KeyCode::KP1;
			case VK_NUMPAD2: return KeyCode::KP2;
			case VK_NUMPAD3: return KeyCode::KP3;
			case VK_NUMPAD4: return KeyCode::KP4;
			case VK_NUMPAD5: return KeyCode::KP5;
			case VK_NUMPAD6: return KeyCode::KP6;
			case VK_NUMPAD7: return KeyCode::KP7;
			case VK_NUMPAD8: return KeyCode::KP8;
			case VK_NUMPAD9: return KeyCode::KP9;
			case VK_DECIMAL: return KeyCode::KPDecimal;
			case VK_DIVIDE: return KeyCode::KPDivide;
			case VK_MULTIPLY: return KeyCode::KPMultiply;
			case VK_SUBTRACT: return KeyCode::KPSubtract;
			case VK_ADD:
				return KeyCode::KPAdd;
				// case :			return KeyCode::KPEnter;
				// case :			return KeyCode::KPEqual;
			case VK_SPACE: return KeyCode::Space;
			case VK_OEM_7: return KeyCode::Apostrophe;
			case VK_OEM_COMMA: return KeyCode::Comma;
			case VK_OEM_MINUS: return KeyCode::Minus;
			case VK_OEM_PERIOD: return KeyCode::Period;
			case VK_OEM_2: return KeyCode::Slash;
			case VK_OEM_1: return KeyCode::Semicolon;
			case VK_OEM_PLUS: return KeyCode::Equal;
			case VK_OEM_4: return KeyCode::LeftBracket;
			case VK_OEM_6: return KeyCode::RightBracket;
			case VK_OEM_5: return KeyCode::Backslash;
			case VK_OEM_3:
				return KeyCode::Tilde;
				// case :			return KeyCode::Menu;
		}
		return KeyCode::Unknown;
	}

	MouseCode PlatformWin32::GetMouseCode(WPARAM wParam, LPARAM lParam)
	{
		// This is only used for X1 and X2 buttons
		switch (GET_XBUTTON_WPARAM(wParam)) {
			case XBUTTON1: return MouseCode::ButtonX1;
			case XBUTTON2: return MouseCode::ButtonX2;
		}
		return MouseCode::Unknown;
	}
}

#endif