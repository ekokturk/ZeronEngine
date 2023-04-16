// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_GLFW
#	include <Platform/API/Shared/GLFW/GLFWHelpers.h>

#	if ZE_PLATFORM_WIN32
#		define GLFW_EXPOSE_NATIVE_WIN32
#	elif ZE_PLATFORM_LINUX
#		define GLFW_EXPOSE_NATIVE_X11
#	endif

#	include <GLFW/glfw3.h>
#	include <GLFW/glfw3native.h>

#	ifdef Button5
#		undef Button5
#	endif

namespace Zeron
{

	SystemHandle GLFWHelpers::GetPlatformWindowHandle(GLFWwindow* glfwWindow)
	{
		SystemHandle handle;
#	if ZE_PLATFORM_WIN32
		handle.mWindow = glfwGetWin32Window(glfwWindow);
#	elif ZE_PLATFORM_LINUX
		handle.mWindow = glfwGetX11Window(glfwWindow);
		handle.mConnection = glfwGetX11Display();
#	endif
		return handle;
	}

	KeyCode GLFWHelpers::GetKeyCode(int code)
	{
		switch (code) {
			case GLFW_KEY_UNKNOWN: return KeyCode::Unknown;
			case GLFW_KEY_0: return KeyCode::N0;
			case GLFW_KEY_1: return KeyCode::N1;
			case GLFW_KEY_2: return KeyCode::N2;
			case GLFW_KEY_3: return KeyCode::N3;
			case GLFW_KEY_4: return KeyCode::N4;
			case GLFW_KEY_5: return KeyCode::N5;
			case GLFW_KEY_6: return KeyCode::N6;
			case GLFW_KEY_7: return KeyCode::N7;
			case GLFW_KEY_8: return KeyCode::N8;
			case GLFW_KEY_9: return KeyCode::N9;
			case GLFW_KEY_A: return KeyCode::A;
			case GLFW_KEY_B: return KeyCode::B;
			case GLFW_KEY_C: return KeyCode::C;
			case GLFW_KEY_D: return KeyCode::D;
			case GLFW_KEY_E: return KeyCode::E;
			case GLFW_KEY_F: return KeyCode::F;
			case GLFW_KEY_G: return KeyCode::G;
			case GLFW_KEY_H: return KeyCode::H;
			case GLFW_KEY_I: return KeyCode::I;
			case GLFW_KEY_J: return KeyCode::J;
			case GLFW_KEY_K: return KeyCode::K;
			case GLFW_KEY_L: return KeyCode::L;
			case GLFW_KEY_M: return KeyCode::M;
			case GLFW_KEY_N: return KeyCode::N;
			case GLFW_KEY_O: return KeyCode::O;
			case GLFW_KEY_P: return KeyCode::P;
			case GLFW_KEY_Q: return KeyCode::Q;
			case GLFW_KEY_R: return KeyCode::R;
			case GLFW_KEY_S: return KeyCode::S;
			case GLFW_KEY_T: return KeyCode::T;
			case GLFW_KEY_U: return KeyCode::U;
			case GLFW_KEY_V: return KeyCode::V;
			case GLFW_KEY_W: return KeyCode::W;
			case GLFW_KEY_X: return KeyCode::X;
			case GLFW_KEY_Y: return KeyCode::Y;
			case GLFW_KEY_Z: return KeyCode::Z;
			case GLFW_KEY_ESCAPE: return KeyCode::Escape;
			case GLFW_KEY_ENTER: return KeyCode::Enter;
			case GLFW_KEY_TAB: return KeyCode::Tab;
			case GLFW_KEY_BACKSPACE: return KeyCode::Backspace;
			case GLFW_KEY_INSERT: return KeyCode::Insert;
			case GLFW_KEY_DELETE: return KeyCode::Delete;
			case GLFW_KEY_LEFT_SHIFT: return KeyCode::LeftShift;
			case GLFW_KEY_LEFT_CONTROL: return KeyCode::LeftControl;
			case GLFW_KEY_LEFT_ALT: return KeyCode::LeftAlt;
			case GLFW_KEY_LEFT_SUPER: return KeyCode::LeftSuper;
			case GLFW_KEY_RIGHT_SHIFT: return KeyCode::RightShift;
			case GLFW_KEY_RIGHT_CONTROL: return KeyCode::RightControl;
			case GLFW_KEY_RIGHT_ALT: return KeyCode::RightAlt;
			case GLFW_KEY_RIGHT_SUPER: return KeyCode::RightSuper;
			case GLFW_KEY_RIGHT: return KeyCode::Right;
			case GLFW_KEY_LEFT: return KeyCode::Left;
			case GLFW_KEY_DOWN: return KeyCode::Down;
			case GLFW_KEY_UP: return KeyCode::Up;
			case GLFW_KEY_PAGE_UP: return KeyCode::PageUp;
			case GLFW_KEY_PAGE_DOWN: return KeyCode::PageDown;
			case GLFW_KEY_HOME: return KeyCode::Home;
			case GLFW_KEY_END: return KeyCode::End;
			case GLFW_KEY_CAPS_LOCK: return KeyCode::CapsLock;
			case GLFW_KEY_SCROLL_LOCK: return KeyCode::ScrollLock;
			case GLFW_KEY_NUM_LOCK: return KeyCode::NumLock;
			case GLFW_KEY_PRINT_SCREEN: return KeyCode::PrintScreen;
			case GLFW_KEY_PAUSE: return KeyCode::Pause;
			case GLFW_KEY_F1: return KeyCode::F1;
			case GLFW_KEY_F2: return KeyCode::F2;
			case GLFW_KEY_F3: return KeyCode::F3;
			case GLFW_KEY_F4: return KeyCode::F4;
			case GLFW_KEY_F5: return KeyCode::F5;
			case GLFW_KEY_F6: return KeyCode::F6;
			case GLFW_KEY_F7: return KeyCode::F7;
			case GLFW_KEY_F8: return KeyCode::F8;
			case GLFW_KEY_F9: return KeyCode::F9;
			case GLFW_KEY_F10: return KeyCode::F10;
			case GLFW_KEY_F11: return KeyCode::F11;
			case GLFW_KEY_F12: return KeyCode::F12;
			case GLFW_KEY_F13: return KeyCode::F13;
			case GLFW_KEY_F14: return KeyCode::F14;
			case GLFW_KEY_F15: return KeyCode::F15;
			case GLFW_KEY_F16: return KeyCode::F16;
			case GLFW_KEY_F17: return KeyCode::F17;
			case GLFW_KEY_F18: return KeyCode::F18;
			case GLFW_KEY_F19: return KeyCode::F19;
			case GLFW_KEY_F20: return KeyCode::F20;
			case GLFW_KEY_F21: return KeyCode::F21;
			case GLFW_KEY_F22: return KeyCode::F22;
			case GLFW_KEY_F23: return KeyCode::F23;
			case GLFW_KEY_F24: return KeyCode::F24;
			case GLFW_KEY_F25: return KeyCode::F25;
			case GLFW_KEY_KP_0: return KeyCode::KP0;
			case GLFW_KEY_KP_1: return KeyCode::KP1;
			case GLFW_KEY_KP_2: return KeyCode::KP2;
			case GLFW_KEY_KP_3: return KeyCode::KP3;
			case GLFW_KEY_KP_4: return KeyCode::KP4;
			case GLFW_KEY_KP_5: return KeyCode::KP5;
			case GLFW_KEY_KP_6: return KeyCode::KP6;
			case GLFW_KEY_KP_7: return KeyCode::KP7;
			case GLFW_KEY_KP_8: return KeyCode::KP8;
			case GLFW_KEY_KP_9: return KeyCode::KP9;
			case GLFW_KEY_KP_DECIMAL: return KeyCode::KPDecimal;
			case GLFW_KEY_KP_DIVIDE: return KeyCode::KPDivide;
			case GLFW_KEY_KP_MULTIPLY: return KeyCode::KPMultiply;
			case GLFW_KEY_KP_SUBTRACT: return KeyCode::KPSubtract;
			case GLFW_KEY_KP_ADD: return KeyCode::KPAdd;
			case GLFW_KEY_KP_ENTER: return KeyCode::KPEnter;
			case GLFW_KEY_KP_EQUAL: return KeyCode::KPEqual;
			case GLFW_KEY_SPACE: return KeyCode::Space;
			case GLFW_KEY_APOSTROPHE: return KeyCode::Apostrophe;
			case GLFW_KEY_COMMA: return KeyCode::Comma;
			case GLFW_KEY_MINUS: return KeyCode::Minus;
			case GLFW_KEY_PERIOD: return KeyCode::Period;
			case GLFW_KEY_SLASH: return KeyCode::Slash;
			case GLFW_KEY_SEMICOLON: return KeyCode::Semicolon;
			case GLFW_KEY_EQUAL: return KeyCode::Equal;
			case GLFW_KEY_LEFT_BRACKET: return KeyCode::LeftBracket;
			case GLFW_KEY_RIGHT_BRACKET: return KeyCode::RightBracket;
			case GLFW_KEY_BACKSLASH: return KeyCode::Backslash;
			case GLFW_KEY_GRAVE_ACCENT: return KeyCode::Tilde;
			case GLFW_KEY_MENU: return KeyCode::Menu;
			default: return KeyCode::Unknown;
		}
	}

	MouseCode GLFWHelpers::GetMouseCode(int code)
	{
		switch (code) {
			case GLFW_MOUSE_BUTTON_1: return MouseCode::LeftButton;
			case GLFW_MOUSE_BUTTON_2: return MouseCode::RightButton;
			case GLFW_MOUSE_BUTTON_3: return MouseCode::MiddleButton;
			case GLFW_MOUSE_BUTTON_4: return MouseCode::ButtonX1;
			case GLFW_MOUSE_BUTTON_5: return MouseCode::ButtonX2;
			case GLFW_MOUSE_BUTTON_6: return MouseCode::Button5;
			case GLFW_MOUSE_BUTTON_7: return MouseCode::Button6;
			case GLFW_MOUSE_BUTTON_8: return MouseCode::Button7;
			default: return MouseCode::Unknown;
		}
	}
}

#endif