// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "WindowWin32.h"

#define GETSCANCODE(lParam) (lParam & 0x00ff0000) >> 16
#define EXTENDED(lParam) (lParam & 0x01000000) != 0
#define NOTREPEATING(lParam) (lParam & 0x40000000) == 0

namespace Zeron {

	LRESULT CALLBACK WndProcRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	WindowWin32::WindowWin32(const WindowConfig& config)
		: Window(config)
		, mHwnd(nullptr)
		, mIsResizing(false)
		, mSizeMinX(0)
		, mSizeMinY(0)
		, mSizeMaxX(0)
		, mSizeMaxY(0)
	{
		mWindowType = WindowAPI::Win32;
	}
	
	WindowWin32::~WindowWin32()
	{
	#if ZE_WINDOW_WIN32
	#endif
	}

	bool WindowWin32::Init()
	{
#if ZE_WINDOW_WIN32
		HINSTANCE hInstance = GetModuleHandle(nullptr);
		if (hInstance == nullptr) {
			ZE_FAIL("Could not find HINSTANCE of Win32 process!");
			return false;
		}

		WNDCLASS wnd = { 0 };
		wnd.lpfnWndProc = WndProcRouter;
		wnd.hInstance = hInstance;
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.lpszClassName = "Zeron Window";
		wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);

		SetLastError(0);
		RegisterClass(&wnd);
		if (GetLastError() > 0) {
			ZE_FAIL("Win32 window class wasn't registered!");
			return false;
		}

		const Vec2i screenCenter = { GetSystemMetrics(SM_CXSCREEN) / 2 - mSize.X ,
		 GetSystemMetrics(SM_CYSCREEN) / 2 - mSize.Y };
		
		const RECT rect = GetAdjustedRect_(screenCenter, mSize);

		mHwnd = CreateWindowEx(
			0,                              // Optional window styles.
			"Zeron Window",                 // Window class
			mName.c_str(),					// Window text
			WS_OVERLAPPEDWINDOW,            // Window style
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr,
			nullptr,
			hInstance,
			this
		);

		if(mHwnd == nullptr) {
			ZE_FAIL("Win32 window was not created!");
			return false;
		}
		ShowWindow(mHwnd, SW_SHOW);

		// We cache position manually since event is not triggered on creation
		const RECT screenRect = GetScreenRect_();
		OnPositionChanged(screenRect.left, screenRect.top);
		
		return true;
	#else
		ZE_FAIL("Current platform does not support Win32 window!");
		return false;
	#endif
	}


	void WindowWin32::BeginFrame() {
	#if ZE_WINDOW_WIN32
		MSG msg = { 0 };
		while (PeekMessage(&msg, mHwnd, NULL, NULL, PM_REMOVE) == TRUE) {
			if (msg.message == WM_QUIT){
				// TODO: App Terminated event here
				mEventQueue.emplace(std::make_unique<WindowEvent_WindowClosed>());
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	#endif
	}

	void WindowWin32::EndFrame() {
		ClearEventQueue();
	}

	void WindowWin32::SetVisible()
	{
	#if ZE_WINDOW_WIN32
		ShowWindow(mHwnd, SW_SHOW);
	#endif
	}

	void WindowWin32::SetHidden()
	{
	#if ZE_WINDOW_WIN32
		ShowWindow(mHwnd, SW_HIDE);
	#endif
	}

	void WindowWin32::SetName(const std::string& name)
	{
	#if ZE_WINDOW_WIN32
		mName = name;
		SetWindowText(mHwnd, mName.c_str());
	#endif
	}

	void WindowWin32::SetAspectRatio(int numerator, int denominator)
	{
	#if ZE_WINDOW_WIN32
		if (!IsFullScreen()) {
			ZE_FAIL("WindowWin32::SetAspectRatio Not implemented!");
		}
	#endif
	}

	void WindowWin32::SetSize(int width, int height)
	{
	#if ZE_WINDOW_WIN32
		if (!IsFullScreen()) {
			const RECT rect = GetAdjustedRect_(mPos, {width, height});
			SetWindowPos(mHwnd, HWND_TOP, 
				rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
				SWP_NOMOVE | SWP_NOOWNERZORDER);
		}
	#endif
	}

	void WindowWin32::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight)
	{
	#if ZE_WINDOW_WIN32
		if (minWidth < maxWidth && minHeight < maxHeight) {
			mSizeMinX = minWidth;
			mSizeMaxX = maxWidth;
			mSizeMinY = minHeight;
			mSizeMaxY = maxHeight;
			SetSize(mSize.X, mSize.Y);
		}
	#endif
	}

	void WindowWin32::SetScreenPosition(int posX, int posY)
	{
	#if ZE_WINDOW_WIN32
		if(!IsFullScreen()) {
			const RECT rect = GetAdjustedRect_({posX, posY}, mSize);
			SetWindowPos(mHwnd, HWND_TOP, 
				rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top,
				SWP_NOSIZE | SWP_NOOWNERZORDER);
		}
	#endif
	}

	void WindowWin32::SetClipCursor(bool shouldClip)
	{
	#if ZE_WINDOW_WIN32
		if (shouldClip) {
			RECT rectPtr = { 0 };
			GetWindowRect(mHwnd, &rectPtr);
			ClipCursor(&rectPtr);
		}
		else {
			ClipCursor(nullptr);
		}
	#endif
	}

	void WindowWin32::OnFullScreenChangedBorderless_()
	{
	#if ZE_WINDOW_WIN32
		if (mIsFullScreen) {
			const HMONITOR hMonitor = MonitorFromWindow(mHwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFOEX monitorInfo = { 0 };
			monitorInfo.cbSize = sizeof(monitorInfo);
			GetMonitorInfo(hMonitor, &monitorInfo);
			const int w = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
			const int h = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

			SetWindowLongPtr(mHwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(mHwnd, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, w, h, SWP_FRAMECHANGED);
		}
		else {
			const RECT rect = GetAdjustedRect_(mPosPrev, mSizePrev);
			SetWindowLongPtr(mHwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
			SetWindowPos(mHwnd, nullptr, rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top, SWP_FRAMECHANGED);
		}
	#endif
	}

	void WindowWin32::OnFullScreenChangedMonitor_()
	{
	#if ZE_WINDOW_WIN32
	#endif
	}

	void* WindowWin32::GetAPIHandle() const
	{
		return GetPlatformHandle();
	}

	void* WindowWin32::GetPlatformHandle() const
	{
	#if ZE_WINDOW_WIN32
		return mHwnd;
	#else
		return nullptr;
	#endif
	}

	void WindowWin32::SetMinimized()
	{
	#if ZE_WINDOW_WIN32
		ShowWindow(mHwnd, SW_MINIMIZE);
	#endif
	}

	void WindowWin32::SetMaximized()
	{
	#if ZE_WINDOW_WIN32
		ShowWindow(mHwnd, SW_MAXIMIZE);
	#endif
	}

	void WindowWin32::SetRestored()
	{
	#if ZE_WINDOW_WIN32
		ShowWindow(mHwnd, SW_RESTORE);
	#endif
	}

	void WindowWin32::SetFocused()
	{
	#if ZE_WINDOW_WIN32
		SetFocus(mHwnd);
	#endif
	}

	void WindowWin32::SetAttention()
	{
#if ZE_WINDOW_WIN32
		FLASHWINFO fInfo = { 0 };
		fInfo.cbSize = sizeof(fInfo);
		fInfo.hwnd = mHwnd;
		fInfo.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		fInfo.uCount = UINT_MAX;
		fInfo.dwTimeout = 0;
		FlashWindowEx(&fInfo);
#endif
	}

#if ZE_WINDOW_WIN32
	LRESULT CALLBACK WndProcRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		WindowWin32* window = nullptr;
		if (msg == WM_CREATE) {
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			window = reinterpret_cast<WindowWin32*>(cs->lpCreateParams);
			SetLastError(0);
			if (SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window)) == 0) {
				if (GetLastError() != FALSE) {
					return FALSE;
				}
			}
		}
		else {
			window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		if (window) {
			return window->WndProc(msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK WindowWin32::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
			case WM_CREATE: {
			} break;
			case WM_PAINT: {
				PAINTSTRUCT ps = { 0 };
				HDC hdc = BeginPaint(mHwnd, &ps);

				FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

				EndPaint(mHwnd, &ps);
			} break;
			case WM_CLOSE: {
				mEventQueue.emplace(std::make_unique<WindowEvent_WindowClosed>());
				// We don't want to trigger WM_DESTROY so we return from here
				return 0;
			} break;
			case WM_DESTROY: {
				//PostQuitMessage(0);
			} break;
			case WM_KEYDOWN: {
				if(NOTREPEATING(lParam)) {
					mEventQueue.emplace(std::make_unique<WindowEvent_KeyDown>(GetKeyCodeWin32_(wParam, lParam)));
				}
			} break;
			case WM_KEYUP: {
				mEventQueue.emplace(std::make_unique<WindowEvent_KeyUp>(GetKeyCodeWin32_(wParam, lParam)));
			} break;
			case WM_CHAR: {
				const unsigned char utf8Char = static_cast<unsigned char>(wctob(static_cast<wchar_t>(wParam)));
				mEventQueue.emplace(std::make_unique<WindowEvent_TextChar>(utf8Char));
			}
			case WM_SYSKEYDOWN: {
				// We don't want window to go into menu loop when ALT or F10 is pressed
				if (NOTREPEATING(lParam)) {
					if (wParam == VK_MENU) {
						mEventQueue.emplace(std::make_unique<WindowEvent_KeyDown>(EXTENDED(lParam) ?
							KeyCode::RightAlt : KeyCode::LeftAlt));
						return 0;
					}
					else if (wParam == VK_F10) {
						mEventQueue.emplace(std::make_unique<WindowEvent_KeyDown>(KeyCode::F10));
						return 0;
					}
				}
			} break;
			case WM_SYSKEYUP: {
				if (NOTREPEATING(lParam)) {
					if (wParam == VK_MENU) {
						mEventQueue.emplace(std::make_unique<WindowEvent_KeyUp>(EXTENDED(lParam) ?
							KeyCode::RightAlt : KeyCode::LeftAlt));
						return 0;
					}
					else if (wParam == VK_F10) {
						mEventQueue.emplace(std::make_unique<WindowEvent_KeyUp>(KeyCode::F10));
						return 0;
					}
				}
			} break;
			case WM_LBUTTONDOWN: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseDown>(MouseCode::LeftButton));
			} break;
			case WM_LBUTTONUP: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseUp>(MouseCode::LeftButton));
			} break;
			case WM_RBUTTONDOWN: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseDown>(MouseCode::RightButton));
			} break;
			case WM_RBUTTONUP: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseUp>(MouseCode::RightButton));
			} break;
			case WM_MBUTTONDOWN: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseDown>(MouseCode::MiddleButton));
			} break;
			case WM_MBUTTONUP: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseUp>(MouseCode::MiddleButton));
			} break;
			case WM_XBUTTONDOWN: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseDown>(GetMouseCodeWin32_(wParam, lParam)));
			} break;
			case WM_XBUTTONUP: {
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseUp>(GetMouseCodeWin32_(wParam, lParam)));
			} break;
			case WM_MOUSEWHEEL: {
				const float wheelDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseScrolled>(0.f, wheelDelta));
			} break;
			case WM_MOUSEMOVE: {
				if (mIsHovered) {
					TRACKMOUSEEVENT tme = { 0 };
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_HOVER | TME_LEAVE; 
					tme.dwHoverTime = 1;
					tme.hwndTrack = mHwnd;
					TrackMouseEvent(&tme);
					mEventQueue.emplace(std::make_unique<WindowEvent_MouseMoved>(LOWORD(lParam), HIWORD(lParam)));
				}
				else {
					OnHoverChanged(true);
					mEventQueue.emplace(std::make_unique<WindowEvent_MouseEnter>());
				}
			} break;
			case WM_MOUSEHOVER: {
				// Hover event is tracked with mouse movement
			} break;
			case WM_MOUSELEAVE: {
				OnHoverChanged(false);
				mEventQueue.emplace(std::make_unique<WindowEvent_MouseExit>());
			} break;
			case WM_SETFOCUS: {
				OnFocusChanged(true);
				mEventQueue.emplace(std::make_unique<WindowEvent_WindowFocused>());
			} break;
			case WM_KILLFOCUS: {
				OnFocusChanged(false);
				mEventQueue.emplace(std::make_unique<WindowEvent_WindowUnfocused>());
			} break;
			case WM_WINDOWPOSCHANGED: {
				// Handle visibility, repositioning and resizing here
				const PWINDOWPOS dataPtr = reinterpret_cast<PWINDOWPOS>(lParam);
				const UINT flags = dataPtr->flags;

				// WM_SHOWWINDOW
				if ((flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW) {
					OnVisibilityChanged(true);
				}
				else if ((flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW) {
					OnVisibilityChanged(false);
				}

				//// SIZE
				if ((flags & SWP_NOSIZE) != SWP_NOSIZE) {
					if (IsIconic(mHwnd)) {
						if (IsMinimized()) {
							OnRestored();
						}
						else {
							// We don't want to change cached position and size for minimized
							// NOTE: We can zero out the size here
							OnMinimized();
							return 0;
						}
					}
					else if (IsZoomed(mHwnd)) {
						if (IsMaximized()) {
							OnRestored();
						}
						else {
							OnMaximized();
							mEventQueue.emplace(std::make_unique<WindowEvent_WindowMaximized>());
						}
					}
					else {
						OnRestored();
					}
					const RECT rect = GetScreenRect_();
					const int width = rect.right - rect.left;
					const int height = rect.bottom - rect.top;
					OnSizeChanged(width, height);
					// We don't want to send this repeatedly if we are manually resizing the window
					if(!mIsResizing) {
						mEventQueue.emplace(std::make_unique<WindowEvent_WindowResized>(mSize.X, mSize.Y));
					}
				}

				// MOVE
				if ((flags & SWP_NOMOVE) != SWP_NOMOVE) {
					const RECT rect = GetScreenRect_();
					OnPositionChanged(rect.left, rect.top);
					mEventQueue.emplace(std::make_unique<WindowEvent_WindowMoved>(rect.left, rect.top));
				}
				return 0;
			}
			case WM_ENTERSIZEMOVE: {
				mIsResizing = true;
			} break;
			case WM_EXITSIZEMOVE: {
				mIsResizing = false;
				mEventQueue.emplace(std::make_unique<WindowEvent_WindowResized>(mSize.X, mSize.Y));
			} break;
			case WM_GETMINMAXINFO:
			{
				if (mSizeMaxX > 0 && mSizeMaxY > 0 && mSizeMinX > 0 && mSizeMinY > 0) {
					MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
					mmi->ptMinTrackSize.x = mSizeMinX;
					mmi->ptMinTrackSize.y = mSizeMinY;
					mmi->ptMaxTrackSize.x = mSizeMaxX;
					mmi->ptMaxTrackSize.y = mSizeMaxY;
					int newWidth = mSize.X;
					int newHeight = mSize.Y;
					if (mSize.X > mSizeMaxX) {
						newWidth = mSizeMaxX;
					}
					else if(mSize.X < mSizeMinX) {
						newWidth = mSizeMinX;
					}
					if (mSize.Y > mSizeMaxY) {
						newHeight = mSizeMaxY;
					}
					else if (mSize.Y < mSizeMinY) {
						newHeight = mSizeMinY;
					}

					if(newWidth != mSize.X || newHeight != mSize.Y) {
						OnSizeChanged(mSize.X, mSize.Y);
					}
					return 0;
				}
			}
			case WM_DISPLAYCHANGE: {
				// TODO: Implement this for resolution change, monitor plugged/unplugged
			} break;
			case WM_DPICHANGED: {
				// TODO: Implement this for DPI change
			} break;
		}
		return DefWindowProc(mHwnd, msg, wParam, lParam);
	}

	RECT WindowWin32::GetScreenRect_() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);
		MapWindowPoints(mHwnd, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
		return rect;
	}

	RECT WindowWin32::GetAdjustedRect_(const Vec2i& position, const Vec2i& size) const
	{
		RECT rect;
		rect.left = position.X;
		rect.top = position.Y;
		rect.right = rect.left + size.X;
		rect.bottom = rect.top + size.Y;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		return rect;
	}

	KeyCode WindowWin32::GetKeyCodeWin32_(WPARAM wParam, LPARAM lParam) {
		switch (wParam) {
		case 0x30:			return KeyCode::N0;
		case 0x31:			return KeyCode::N1;
		case 0x32:			return KeyCode::N2;
		case 0x33:			return KeyCode::N3;
		case 0x34:			return KeyCode::N4;
		case 0x35:			return KeyCode::N5;
		case 0x36:			return KeyCode::N6;
		case 0x37:			return KeyCode::N7;
		case 0x38:			return KeyCode::N8;
		case 0x39:			return KeyCode::N9;
		case 0x41:			return KeyCode::A;
		case 0x42:			return KeyCode::B;
		case 0x43:			return KeyCode::C;
		case 0x44:			return KeyCode::D;
		case 0x45:			return KeyCode::E;
		case 0x46:			return KeyCode::F;
		case 0x47:			return KeyCode::G;
		case 0x48:			return KeyCode::H;
		case 0x49:			return KeyCode::I;
		case 0x4A:			return KeyCode::J;
		case 0x4B:			return KeyCode::K;
		case 0x4C:			return KeyCode::L;
		case 0x4D:			return KeyCode::M;
		case 0x4E:			return KeyCode::N;
		case 0x4F:			return KeyCode::O;
		case 0x50:			return KeyCode::P;
		case 0x51:			return KeyCode::Q;
		case 0x52:			return KeyCode::R;
		case 0x53:			return KeyCode::S;
		case 0x54:			return KeyCode::T;
		case 0x55:			return KeyCode::U;
		case 0x56:			return KeyCode::V;
		case 0x57:			return KeyCode::W;
		case 0x58:			return KeyCode::X;
		case 0x59:			return KeyCode::Y;
		case 0x5A:			return KeyCode::Z;
		case VK_ESCAPE:		return KeyCode::Escape;
		case VK_RETURN:		return KeyCode::Enter;
		case VK_TAB:		return KeyCode::Tab;
		case VK_BACK:		return KeyCode::Backspace;
		case VK_INSERT:		return KeyCode::Insert;
		case VK_DELETE:		return KeyCode::Delete;
		case VK_SHIFT: {
			switch (MapVirtualKey(GETSCANCODE(lParam), MAPVK_VSC_TO_VK_EX)) {
			case VK_LSHIFT: return KeyCode::LeftShift;
			case VK_RSHIFT: return KeyCode::RightShift;
			}
		} break;
		case VK_CONTROL:	return EXTENDED(lParam) ? KeyCode::RightControl : KeyCode::LeftControl;
		case VK_MENU:		return EXTENDED(lParam) ? KeyCode::RightAlt : KeyCode::LeftAlt;
		case VK_LWIN:		return KeyCode::LeftSuper;
		case VK_RWIN:		return KeyCode::RightSuper;
		case VK_RIGHT:		return KeyCode::Right;
		case VK_LEFT:		return KeyCode::Left;
		case VK_DOWN:		return KeyCode::Down;
		case VK_UP:			return KeyCode::Up;
		case VK_PRIOR:		return KeyCode::PageUp;
		case VK_NEXT:		return KeyCode::PageDown;
		case VK_HOME:		return KeyCode::Home;
		case VK_END:		return KeyCode::End;
		case VK_CAPITAL:	return KeyCode::CapsLock;
		case VK_SCROLL:		return KeyCode::ScrollLock;
		case VK_NUMLOCK:	return KeyCode::NumLock;
		case VK_SNAPSHOT:	return KeyCode::PrintScreen;
		case VK_PAUSE:		return KeyCode::Pause;
		case VK_F1:			return KeyCode::F1;
		case VK_F2:			return KeyCode::F2;
		case VK_F3:			return KeyCode::F3;
		case VK_F4:			return KeyCode::F4;
		case VK_F5:			return KeyCode::F5;
		case VK_F6:			return KeyCode::F6;
		case VK_F7:			return KeyCode::F7;
		case VK_F8:			return KeyCode::F8;
		case VK_F9:			return KeyCode::F9;
		case VK_F10:		return KeyCode::F10;
		case VK_F11:		return KeyCode::F11;
		case VK_F12:		return KeyCode::F12;
		case VK_F13:		return KeyCode::F13;
		case VK_F14:		return KeyCode::F14;
		case VK_F15:		return KeyCode::F15;
		case VK_F16:		return KeyCode::F16;
		case VK_F17:		return KeyCode::F17;
		case VK_F18:		return KeyCode::F18;
		case VK_F19:		return KeyCode::F19;
		case VK_F20:		return KeyCode::F20;
		case VK_F21:		return KeyCode::F21;
		case VK_F22:		return KeyCode::F22;
		case VK_F23:		return KeyCode::F23;
		case VK_F24:		return KeyCode::F24;
		case VK_NUMPAD0:	return KeyCode::KP0;
		case VK_NUMPAD1:	return KeyCode::KP1;
		case VK_NUMPAD2:	return KeyCode::KP2;
		case VK_NUMPAD3:	return KeyCode::KP3;
		case VK_NUMPAD4:	return KeyCode::KP4;
		case VK_NUMPAD5:	return KeyCode::KP5;
		case VK_NUMPAD6:	return KeyCode::KP6;
		case VK_NUMPAD7:	return KeyCode::KP7;
		case VK_NUMPAD8:	return KeyCode::KP8;
		case VK_NUMPAD9:	return KeyCode::KP9;
		case VK_DECIMAL:	return KeyCode::KPDecimal;
		case VK_DIVIDE:		return KeyCode::KPDivide;
		case VK_MULTIPLY:	return KeyCode::KPMultiply;
		case VK_SUBTRACT:	return KeyCode::KPSubtract;
		case VK_ADD:		return KeyCode::KPAdd;
		//case :			return KeyCode::KPEnter;
		//case :			return KeyCode::KPEqual;
		case VK_SPACE:		return KeyCode::Space;
		case VK_OEM_7:		return KeyCode::Apostrophe;
		case VK_OEM_COMMA:	return KeyCode::Comma;
		case VK_OEM_MINUS:	return KeyCode::Minus;
		case VK_OEM_PERIOD: return KeyCode::Period;
		case VK_OEM_2:		return KeyCode::Slash;
		case VK_OEM_1:		return KeyCode::Semicolon;
		case VK_OEM_PLUS:	return KeyCode::Equal;
		case VK_OEM_4:		return KeyCode::LeftBracket;
		case VK_OEM_6:		return KeyCode::RightBracket;
		case VK_OEM_5:		return KeyCode::Backslash;
		case VK_OEM_3:		return KeyCode::Tilde;
		//case :			return KeyCode::Menu;
		}
		return KeyCode::Unknown;
	}

	MouseCode WindowWin32::GetMouseCodeWin32_(WPARAM wParam, LPARAM lParam) 
	{
		// This is only used for X1 and X2 buttons
		switch (GET_XBUTTON_WPARAM(wParam)) {
		case XBUTTON1: return MouseCode::ButtonX1;
		case XBUTTON2: return MouseCode::ButtonX2;
		}
		return MouseCode::Unknown;
	}
#endif

}
