// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_GLFW
#include <Window/API/GLFW/WindowGLFW.h>

#include <Window/API/GLFW/GLFWHelpers.h>
#include <GLFW/glfw3.h>

namespace Zeron {

	int WindowGLFW::mWindowGLFWCount = 0;

	WindowGLFW::WindowGLFW(const WindowConfig& config)
		: Window(config, WindowAPI::GLFW)
		, mWindowGLFW(nullptr)
		, mMonitorGLFW(nullptr)
		, mCursorGLFW(nullptr)
		, mIsResizing(false)
		, mIsCursorClipped(false)
	{
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

	bool WindowGLFW::Init()
	{
		glfwSetErrorCallback([](int errorCode, const char* errorMessage) {
			ZE_LOGE("GLFW ERROR {}: {}", errorCode, errorMessage);
		});

		if (mWindowGLFWCount == 0) {
			if (glfwInit() == GLFW_FALSE) {
				ZE_FAIL("GLFW was not initialized!");
				return false;
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
			return false;
		}

		glfwSetWindowUserPointer(mWindowGLFW, this);

		// We need to initialize cached postion
		glfwGetWindowPos(mWindowGLFW, &mPos.X, &mPos.Y);
		mPosPrev = mPos;

		_registerEvents();
		
		mWindowGLFWCount++;
		return true;
	}

	void WindowGLFW::Update()
	{
		ClearEventQueue();
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

		glfwPollEvents();

		// Queue blocked until resizing is over so we send the final size with the event
		if(mIsResizing) {
			QueueEvent(std::make_unique<WindowEvent_WindowResized>(mSize.X, mSize.Y));
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

	void WindowGLFW::QueueEvent(std::unique_ptr<WindowEvent> e) {
		mEventQueue.emplace(std::move(e));
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

	void WindowGLFW::_registerEvents()
	{
	// ------------------------ WINDOW EVENTS --------------------------
		// WINDOW RESIZE
		glfwSetWindowSizeCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int width, int height) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			const int canResize = glfwGetWindowAttrib(windowGLFW, GLFW_RESIZABLE);
			// Size of the window is not changed on minimize
			if (!window->IsMinimized() && canResize == GLFW_TRUE) {
				window->OnSizeChanged(width, height);
				window->mIsResizing = true;
			}
			});

		// WINDOW REPOSITION
		glfwSetWindowPosCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int posX, int posY) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			// Here is ideal to handle Minimize/Maximize since this event is triggered before them
			if (glfwGetWindowAttrib(windowGLFW, GLFW_ICONIFIED) == GLFW_TRUE) {
				window->OnMinimized();
				window->QueueEvent(std::make_unique<WindowEvent_WindowMinimized>());
				// Position of the window is not changed on minimize
				return;
			}
			else {
				if (window->IsMinimized()) {
					if (window->IsFullScreen()) {
						return;
					}
					window->OnRestored();
				}
			}
			if (glfwGetWindowAttrib(windowGLFW, GLFW_MAXIMIZED) == GLFW_TRUE) {
				window->OnMaximized();
				window->QueueEvent(std::make_unique<WindowEvent_WindowMaximized>());
			}
			else {
				if (window->IsMaximized()) {
					window->OnRestored();
				}
			}
			window->OnPositionChanged(posX, posY);
			window->QueueEvent(std::make_unique<WindowEvent_WindowMoved>(posX, posY));
			});

		// WINDOW CLOSED
		glfwSetWindowCloseCallback(mWindowGLFW, [](GLFWwindow* windowGLFW) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			window->QueueEvent(std::make_unique<WindowEvent_WindowClosed>());
			});

		// WINDOW FOCUSED/UNFOCUSED
		glfwSetWindowFocusCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int isFocused) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			switch (isFocused) {
			case GLFW_TRUE: {
				window->OnFocusChanged(true);
				window->QueueEvent(std::make_unique<WindowEvent_WindowFocused>());
			} break;
			case GLFW_FALSE: {
				window->OnFocusChanged(false);
				window->QueueEvent(std::make_unique<WindowEvent_WindowUnfocused>());
			} break;
			default:;
			}
		});

		//glfwSetFramebufferSizeCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int scaleX, int scaleY) {
		//	WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
		//});

	// ------------------------ MOUSE EVENTS ---------------------------
		// MOUSE BUTTON PRESSED
		glfwSetMouseButtonCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int buttonIndex, int actionType, int modifiers) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			switch (actionType) {
				case GLFW_PRESS: {
					window->QueueEvent(std::make_unique<WindowEvent_MouseDown>(_getMouseCodeGLFW(buttonIndex)));
				} break;
				case GLFW_RELEASE: {
					window->QueueEvent(std::make_unique<WindowEvent_MouseUp>(_getMouseCodeGLFW(buttonIndex)));
				} break;
				default:;
			}
		});
		
		// MOUSE MOVED
		glfwSetCursorPosCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, double posX, double posY){
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			if(window->IsHovered()) {
				window->QueueEvent(std::make_unique<WindowEvent_MouseMoved>(static_cast<int>(posX), static_cast<int>(posY)));
			}
		});

		// MOUSE SCROLLED
		glfwSetScrollCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, double offsetX, double offsetY) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			window->QueueEvent(std::make_unique<WindowEvent_MouseScrolled>(static_cast<float>(offsetX), static_cast<float>(offsetY)));
		});

		// MOUSE ENTER/EXIT
		glfwSetCursorEnterCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int isEntered) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			if (window->IsFocused() && window->IsCursorClipped()) {
				return;
			}

			switch (isEntered) {
			case GLFW_TRUE: {
				window->OnHoverChanged(true);
				window->QueueEvent(std::make_unique<WindowEvent_MouseEnter>());
			} break;
			case GLFW_FALSE: {
				window->OnHoverChanged(false);
				window->QueueEvent(std::make_unique<WindowEvent_MouseExit>());
			} break;
			default:;
			}
		});

	// --------------------------- KEY EVENTS --------------------------
		glfwSetKeyCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, int keyIndex, int scanCode, int actionType, int modifierMask) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			switch (actionType) {
				case GLFW_PRESS:{
					window->QueueEvent(std::make_unique<WindowEvent_KeyDown>(_getKeyCodeGLFW(keyIndex)));
				} break;
				case GLFW_RELEASE:{
					window->QueueEvent(std::make_unique<WindowEvent_KeyUp>(_getKeyCodeGLFW(keyIndex)));
				} break;
				default:;
			}
		});

		glfwSetCharModsCallback(mWindowGLFW, [](GLFWwindow* windowGLFW, unsigned int codepoint, int modifierMask) {
			WindowGLFW* window = _getUserPointerGLFW(windowGLFW);
			window->QueueEvent(std::make_unique<WindowEvent_TextChar>(static_cast<unsigned char>(codepoint)));
		});
		
	// ----------------------- GAMEPAD EVENTS --------------------------

		
	}

	WindowGLFW* WindowGLFW::_getUserPointerGLFW(GLFWwindow* windowGLFW)
	{
		ZE_ASSERT(windowGLFW, "GLFW Window user point is not set!");
		return static_cast<WindowGLFW*>(glfwGetWindowUserPointer(windowGLFW));
	}

	KeyCode WindowGLFW::_getKeyCodeGLFW(int code) {
		switch (code) {
		case GLFW_KEY_UNKNOWN:			return KeyCode::Unknown;
		case GLFW_KEY_0:				return KeyCode::N0;
		case GLFW_KEY_1:				return KeyCode::N1;
		case GLFW_KEY_2:				return KeyCode::N2;
		case GLFW_KEY_3:				return KeyCode::N3;
		case GLFW_KEY_4:				return KeyCode::N4;
		case GLFW_KEY_5:				return KeyCode::N5;
		case GLFW_KEY_6:				return KeyCode::N6;
		case GLFW_KEY_7:				return KeyCode::N7;
		case GLFW_KEY_8:				return KeyCode::N8;
		case GLFW_KEY_9:				return KeyCode::N9;
		case GLFW_KEY_A:				return KeyCode::A;
		case GLFW_KEY_B:				return KeyCode::B;
		case GLFW_KEY_C:				return KeyCode::C;
		case GLFW_KEY_D:				return KeyCode::D;
		case GLFW_KEY_E:				return KeyCode::E;
		case GLFW_KEY_F:				return KeyCode::F;
		case GLFW_KEY_G:				return KeyCode::G;
		case GLFW_KEY_H:				return KeyCode::H;
		case GLFW_KEY_I:				return KeyCode::I;
		case GLFW_KEY_J:				return KeyCode::J;
		case GLFW_KEY_K:				return KeyCode::K;
		case GLFW_KEY_L:				return KeyCode::L;
		case GLFW_KEY_M:				return KeyCode::M;
		case GLFW_KEY_N:				return KeyCode::N;
		case GLFW_KEY_O:				return KeyCode::O;
		case GLFW_KEY_P:				return KeyCode::P;
		case GLFW_KEY_Q:				return KeyCode::Q;
		case GLFW_KEY_R:				return KeyCode::R;
		case GLFW_KEY_S:				return KeyCode::S;
		case GLFW_KEY_T:				return KeyCode::T;
		case GLFW_KEY_U:				return KeyCode::U;
		case GLFW_KEY_V:				return KeyCode::V;
		case GLFW_KEY_W:				return KeyCode::W;
		case GLFW_KEY_X:				return KeyCode::X;
		case GLFW_KEY_Y:				return KeyCode::Y;
		case GLFW_KEY_Z:				return KeyCode::Z;
		case GLFW_KEY_ESCAPE:			return KeyCode::Escape;
		case GLFW_KEY_ENTER:			return KeyCode::Enter;
		case GLFW_KEY_TAB:				return KeyCode::Tab;
		case GLFW_KEY_BACKSPACE:		return KeyCode::Backspace;
		case GLFW_KEY_INSERT:			return KeyCode::Insert;
		case GLFW_KEY_DELETE:			return KeyCode::Delete;
		case GLFW_KEY_LEFT_SHIFT:		return KeyCode::LeftShift;
		case GLFW_KEY_LEFT_CONTROL:		return KeyCode::LeftControl;
		case GLFW_KEY_LEFT_ALT:			return KeyCode::LeftAlt;
		case GLFW_KEY_LEFT_SUPER:		return KeyCode::LeftSuper;
		case GLFW_KEY_RIGHT_SHIFT:		return KeyCode::RightShift;
		case GLFW_KEY_RIGHT_CONTROL:	return KeyCode::RightControl;
		case GLFW_KEY_RIGHT_ALT:		return KeyCode::RightAlt;
		case GLFW_KEY_RIGHT_SUPER:		return KeyCode::RightSuper;
		case GLFW_KEY_RIGHT:			return KeyCode::Right;
		case GLFW_KEY_LEFT:				return KeyCode::Left;
		case GLFW_KEY_DOWN:				return KeyCode::Down;
		case GLFW_KEY_UP:				return KeyCode::Up;
		case GLFW_KEY_PAGE_UP:			return KeyCode::PageUp;
		case GLFW_KEY_PAGE_DOWN:		return KeyCode::PageDown;
		case GLFW_KEY_HOME:				return KeyCode::Home;
		case GLFW_KEY_END:				return KeyCode::End;
		case GLFW_KEY_CAPS_LOCK:		return KeyCode::CapsLock;
		case GLFW_KEY_SCROLL_LOCK:		return KeyCode::ScrollLock;
		case GLFW_KEY_NUM_LOCK:			return KeyCode::NumLock;
		case GLFW_KEY_PRINT_SCREEN:		return KeyCode::PrintScreen;
		case GLFW_KEY_PAUSE:			return KeyCode::Pause;
		case GLFW_KEY_F1:				return KeyCode::F1;
		case GLFW_KEY_F2:				return KeyCode::F2;
		case GLFW_KEY_F3:				return KeyCode::F3;
		case GLFW_KEY_F4:				return KeyCode::F4;
		case GLFW_KEY_F5:				return KeyCode::F5;
		case GLFW_KEY_F6:				return KeyCode::F6;
		case GLFW_KEY_F7:				return KeyCode::F7;
		case GLFW_KEY_F8:				return KeyCode::F8;
		case GLFW_KEY_F9:				return KeyCode::F9;
		case GLFW_KEY_F10:				return KeyCode::F10;
		case GLFW_KEY_F11:				return KeyCode::F11;
		case GLFW_KEY_F12:				return KeyCode::F12;
		case GLFW_KEY_F13:				return KeyCode::F13;
		case GLFW_KEY_F14:				return KeyCode::F14;
		case GLFW_KEY_F15:				return KeyCode::F15;
		case GLFW_KEY_F16:				return KeyCode::F16;
		case GLFW_KEY_F17:				return KeyCode::F17;
		case GLFW_KEY_F18:				return KeyCode::F18;
		case GLFW_KEY_F19:				return KeyCode::F19;
		case GLFW_KEY_F20:				return KeyCode::F20;
		case GLFW_KEY_F21:				return KeyCode::F21;
		case GLFW_KEY_F22:				return KeyCode::F22;
		case GLFW_KEY_F23:				return KeyCode::F23;
		case GLFW_KEY_F24:				return KeyCode::F24;
		case GLFW_KEY_F25:				return KeyCode::F25;
		case GLFW_KEY_KP_0:				return KeyCode::KP0;
		case GLFW_KEY_KP_1:				return KeyCode::KP1;
		case GLFW_KEY_KP_2:				return KeyCode::KP2;
		case GLFW_KEY_KP_3:				return KeyCode::KP3;
		case GLFW_KEY_KP_4:				return KeyCode::KP4;
		case GLFW_KEY_KP_5:				return KeyCode::KP5;
		case GLFW_KEY_KP_6:				return KeyCode::KP6;
		case GLFW_KEY_KP_7:				return KeyCode::KP7;
		case GLFW_KEY_KP_8:				return KeyCode::KP8;
		case GLFW_KEY_KP_9:				return KeyCode::KP9;
		case GLFW_KEY_KP_DECIMAL:		return KeyCode::KPDecimal;
		case GLFW_KEY_KP_DIVIDE:		return KeyCode::KPDivide;
		case GLFW_KEY_KP_MULTIPLY:		return KeyCode::KPMultiply;
		case GLFW_KEY_KP_SUBTRACT:		return KeyCode::KPSubtract;
		case GLFW_KEY_KP_ADD:			return KeyCode::KPAdd;
		case GLFW_KEY_KP_ENTER:			return KeyCode::KPEnter;
		case GLFW_KEY_KP_EQUAL:			return KeyCode::KPEqual;
		case GLFW_KEY_SPACE:			return KeyCode::Space;
		case GLFW_KEY_APOSTROPHE:		return KeyCode::Apostrophe;
		case GLFW_KEY_COMMA:			return KeyCode::Comma;
		case GLFW_KEY_MINUS:			return KeyCode::Minus;
		case GLFW_KEY_PERIOD:			return KeyCode::Period;
		case GLFW_KEY_SLASH:			return KeyCode::Slash;
		case GLFW_KEY_SEMICOLON:		return KeyCode::Semicolon;
		case GLFW_KEY_EQUAL:			return KeyCode::Equal;
		case GLFW_KEY_LEFT_BRACKET:		return KeyCode::LeftBracket;
		case GLFW_KEY_RIGHT_BRACKET:	return KeyCode::RightBracket;
		case GLFW_KEY_BACKSLASH:		return KeyCode::Backslash;
		case GLFW_KEY_GRAVE_ACCENT:		return KeyCode::Tilde;
		case GLFW_KEY_MENU:				return KeyCode::Menu;
		default:						return KeyCode::Unknown;
		}
	}
	
	MouseCode WindowGLFW::_getMouseCodeGLFW(int code)
	{
		switch (code) {
		case GLFW_MOUSE_BUTTON_1:		return MouseCode::LeftButton;
		case GLFW_MOUSE_BUTTON_2:		return MouseCode::RightButton;
		case GLFW_MOUSE_BUTTON_3:		return MouseCode::MiddleButton;
		case GLFW_MOUSE_BUTTON_4:		return MouseCode::ButtonX1;
		case GLFW_MOUSE_BUTTON_5:		return MouseCode::ButtonX2;
		case GLFW_MOUSE_BUTTON_6:		return MouseCode::Button5;
		case GLFW_MOUSE_BUTTON_7:		return MouseCode::Button6;
		case GLFW_MOUSE_BUTTON_8:		return MouseCode::Button7;
		default:						return MouseCode::Unknown;
		}
	}
}

#endif