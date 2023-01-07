// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_WIN32

#include <Core/SystemHandle.h>
#include <Platform/SystemEvent.h>
#include <Platform/API/Win32/PlatformWin32.h>
#include <Platform/API/Win32/SystemEventProcessorWin32.h>
#include <Platform/API/Win32/WindowWin32.h>
#include <Platform/Window.h>
#include <Platform/WindowManager.h>

#define GETSCANCODE(lParam) (lParam & 0x00ff0000) >> 16
#define EXTENDED(lParam) (lParam & 0x01000000) != 0
#define NOTREPEATING(lParam) (lParam & 0x40000000) == 0

namespace Zeron
{
	SystemEventProcessorWin32::SystemEventProcessorWin32(DispatchFn dispatchCallback, ContextFn contextCallback)
		: SystemEventProcessor(std::move(dispatchCallback), std::move(contextCallback))
	{
	}

	void SystemEventProcessorWin32::ProcessEvents()
	{
		MSG msg = {};
		while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE) == TRUE) {
			if (msg.message == WM_QUIT) {
				const SystemEvent::Context ctx = mContextCallback(SystemHandle{ msg.hwnd });
				if (ctx.mWindow) {
					// TODO: App Terminated event here
					mDispatchCallback({ SystemEvent::WindowClosed{} }, ctx);
				}
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	LRESULT SystemEventProcessorWin32::WndProcRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		SystemEventProcessorWin32* eventProcessor = nullptr;
		if (msg == WM_CREATE) {
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			eventProcessor = reinterpret_cast<SystemEventProcessorWin32*>(cs->lpCreateParams);
			SetLastError(0);
			if (SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(eventProcessor)) == 0) {
				if (GetLastError() != FALSE) {
					return FALSE;
				}
			}
		}
		else {
			LONG_PTR userData = GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (userData != NULL) {
				eventProcessor = reinterpret_cast<SystemEventProcessorWin32*>(userData);
			}
		}

		if (eventProcessor) {
			return eventProcessor->WndProc(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT SystemEventProcessorWin32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		const SystemEvent::Context ctx = mContextCallback(SystemHandle{ hWnd });
		if(ctx.mWindow && ctx.mWindow->GetWindowType() == WindowAPI::Win32) {
			const WindowWin32* window = static_cast<WindowWin32*>(ctx.mWindow);

			switch (msg) {
			case WM_CREATE: {
			} break;
			case WM_PAINT: {
				PAINTSTRUCT ps = { 0 };
				HDC hdc = BeginPaint(hWnd, &ps);
				FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
				EndPaint(hWnd, &ps);
			} break;
			case WM_CLOSE: {
				mDispatchCallback({ SystemEvent::WindowClosed{} }, ctx);
				// We don't want to trigger WM_DESTROY so we return from here
				return 0;
			} break;
			case WM_DESTROY: {
				//PostQuitMessage(0);
			} break;
			case WM_KEYDOWN: {
				if (NOTREPEATING(lParam)) {
					mDispatchCallback({ SystemEvent::KeyDown{ PlatformWin32::GetKeyCode(wParam, lParam)} }, ctx);
				}
			} break;
			case WM_KEYUP: {
				mDispatchCallback({ SystemEvent::KeyUp{ PlatformWin32::GetKeyCode(wParam, lParam)} }, ctx);
			} break;
			case WM_CHAR: {
				const unsigned char utf8Char = static_cast<unsigned char>(wctob(static_cast<wchar_t>(wParam)));
				mDispatchCallback({ SystemEvent::TextChar{ utf8Char } }, ctx);
			}
			case WM_SYSKEYDOWN: {
				// We don't want window to go into menu loop when ALT or F10 is pressed
				if (NOTREPEATING(lParam)) {
					if (wParam == VK_MENU) {
						mDispatchCallback({ SystemEvent::KeyDown{ EXTENDED(lParam) ? KeyCode::RightAlt : KeyCode::LeftAlt } }, ctx);
						return 0;
					}
					else if (wParam == VK_F10) {
						mDispatchCallback({ SystemEvent::KeyDown{ KeyCode::F10 } }, ctx);
						return 0;
					}
				}
			} break;
			case WM_SYSKEYUP: {
				if (NOTREPEATING(lParam)) {
					if (wParam == VK_MENU) {
						mDispatchCallback({ SystemEvent::KeyUp{ EXTENDED(lParam) ? KeyCode::RightAlt : KeyCode::LeftAlt } }, ctx);
						return 0;
					}
					else if (wParam == VK_F10) {
						mDispatchCallback({ SystemEvent::KeyUp{ KeyCode::F10 } }, ctx);
						return 0;
					}
				}
			} break;
			case WM_LBUTTONDOWN: {
				mDispatchCallback({ SystemEvent::MouseButtonDown{ MouseCode::LeftButton } }, ctx);
			} break;
			case WM_LBUTTONUP: {
				mDispatchCallback({ SystemEvent::MouseButtonUp{ MouseCode::LeftButton } }, ctx);
			} break;
			case WM_RBUTTONDOWN: {
				mDispatchCallback({ SystemEvent::MouseButtonDown{ MouseCode::RightButton } }, ctx);
			} break;
			case WM_RBUTTONUP: {
				mDispatchCallback({ SystemEvent::MouseButtonUp{ MouseCode::RightButton } }, ctx);
			} break;
			case WM_MBUTTONDOWN: {
				mDispatchCallback({ SystemEvent::MouseButtonDown{ MouseCode::MiddleButton } }, ctx);
			} break;
			case WM_MBUTTONUP: {
				mDispatchCallback({ SystemEvent::MouseButtonUp{ MouseCode::MiddleButton } }, ctx);
			} break;
			case WM_XBUTTONDOWN: {
				mDispatchCallback({ SystemEvent::MouseButtonDown{ PlatformWin32::GetMouseCode(wParam, lParam) } }, ctx);
			} break;
			case WM_XBUTTONUP: {
				mDispatchCallback({ SystemEvent::MouseButtonUp{ PlatformWin32::GetMouseCode(wParam, lParam) } }, ctx);
			} break;
			case WM_MOUSEWHEEL: {
				const float wheelDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
				mDispatchCallback({ SystemEvent::MouseScroll{ 0.f, wheelDelta } }, ctx);
			} break;
			case WM_MOUSEMOVE: {
				if (window->IsHovered()) {
					TRACKMOUSEEVENT tme = { 0 };
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_HOVER | TME_LEAVE;
					tme.dwHoverTime = 1;
					tme.hwndTrack = hWnd;
					TrackMouseEvent(&tme);
					mDispatchCallback({ SystemEvent::MouseMoved{ LOWORD(lParam), HIWORD(lParam) } }, ctx);
				}
				else {
					mDispatchCallback({ SystemEvent::MouseEnter{} }, ctx);
				}
			} break;
			case WM_MOUSEHOVER: {
				// Hover event is tracked with mouse movement
			} break;
			case WM_MOUSELEAVE: {
				mDispatchCallback({ SystemEvent::MouseExit{} }, ctx);
			} break;
			case WM_SETFOCUS: {
				mDispatchCallback({ SystemEvent::WindowFocused{} }, ctx);
			} break;
			case WM_KILLFOCUS: {
				mDispatchCallback({ SystemEvent::WindowUnfocused{} }, ctx);
			} break;
			case WM_WINDOWPOSCHANGED: {
				// Handle visibility, repositioning and resizing here
				const PWINDOWPOS dataPtr = reinterpret_cast<PWINDOWPOS>(lParam);
				const UINT flags = dataPtr->flags;
				// WM_SHOWWINDOW
				if ((flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW) {
					mDispatchCallback({ SystemEvent::WindowVisibilityChanged{ true } }, ctx);
				}
				else if ((flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW) {
					mDispatchCallback({ SystemEvent::WindowVisibilityChanged{ false } }, ctx);
				}

				// SIZE
				if ((flags & SWP_NOSIZE) != SWP_NOSIZE) {
					if (IsIconic(hWnd)) {
						if (window->IsMinimized()) {
							mDispatchCallback({ SystemEvent::WindowRestored{} }, ctx);
						}
						else {
							// We don't want to change cached position and size for minimized
							// NOTE: We can zero out the size here
							mDispatchCallback({ SystemEvent::WindowMinimized{} }, ctx);
							return 0;
						}
					}
					else if (IsZoomed(hWnd)) {
						if (window->IsMaximized()) {
							mDispatchCallback({ SystemEvent::WindowRestored{} }, ctx);
						}
						else {
							mDispatchCallback({ SystemEvent::WindowMaximized{} }, ctx);
						}
					}
					else {
						mDispatchCallback({ SystemEvent::WindowRestored{} }, ctx);
					}
					const RECT rect = window->GetScreenRect();
					const int width = rect.right - rect.left;
					const int height = rect.bottom - rect.top;
					mDispatchCallback({ SystemEvent::WindowSizeChanged{ width, height } }, ctx);
					// We don't want to send this repeatedly if we are manually resizing the window
					if (!window->IsResizing()) {
						mDispatchCallback({ SystemEvent::WindowResized{ width, height } }, ctx);
					}
				}
				// MOVE
				if ((flags & SWP_NOMOVE) != SWP_NOMOVE) {
					const RECT rect = window->GetScreenRect();
					mDispatchCallback({ SystemEvent::WindowMoved{ rect.left, rect.top } }, ctx);
				}
				return 0;
			}
			case WM_ENTERSIZEMOVE: {
				const Vec2i size = window->GetSize();
				mDispatchCallback({ SystemEvent::WindowResizeStarted{ size.X, size.Y } }, ctx);
			} break;
			case WM_EXITSIZEMOVE: {
				const Vec2i size = window->GetSize();
				mDispatchCallback({ SystemEvent::WindowResized{ size.X, size.Y } }, ctx);
			} break;
			case WM_GETMINMAXINFO:
			{
				const Vec2i size = window->GetSize();
				const Vec2i minSize = window->GetSizeMin();
				const Vec2i maxSize = window->GetSizeMax();

				if (maxSize.X > 0 && maxSize.Y > 0 && minSize.X > 0 && minSize.Y > 0) {
					MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
					mmi->ptMinTrackSize.x = minSize.X;
					mmi->ptMinTrackSize.y = minSize.Y;
					mmi->ptMaxTrackSize.x = maxSize.X;
					mmi->ptMaxTrackSize.y = maxSize.Y;
					int newWidth = size.X;
					int newHeight = size.Y;
					if (size.X > maxSize.X) {
						newWidth = maxSize.X;
					}
					else if (size.X < minSize.X) {
						newWidth = minSize.X;
					}
					if (size.Y > maxSize.Y) {
						newHeight = maxSize.Y;
					}
					else if (size.Y < minSize.Y) {
						newHeight = minSize.Y;
					}

					if (newWidth != size.X || newHeight != size.Y) {
						mDispatchCallback({ SystemEvent::WindowSizeChanged{ size.X, size.Y } }, ctx);
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
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

#endif