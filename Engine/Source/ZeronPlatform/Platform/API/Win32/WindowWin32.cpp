// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_WIN32

#include <Platform/API/Win32/WindowWin32.h>

namespace Zeron {
	LRESULT CALLBACK WndProcRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	uint32_t WindowWin32::mWindowIdCounter = 0;

	WindowWin32::WindowWin32(const WindowConfig& config, WNDPROC wndProc, void* userData)
		: Window(config, WindowAPI::Win32)
		, mHwnd(nullptr)
	{
		HINSTANCE hInstance = GetModuleHandle(nullptr);
		if (hInstance == nullptr) {
			ZE_FAIL("Could not find HINSTANCE of Win32 process!");
		}

		mWindowClassName = fmt::format("ZeronWin32_Window_{}", mWindowIdCounter);

		WNDCLASS wnd = {};
		wnd.lpfnWndProc = wndProc;
		wnd.hInstance = hInstance;
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.lpszClassName = mWindowClassName.c_str();
		wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);

		SetLastError(0);
		RegisterClass(&wnd);
		if (GetLastError() > 0) {
			ZE_FAIL("Win32 window class wasn't registered!");
		}

		const Vec2i screenCenter = { GetSystemMetrics(SM_CXSCREEN) / 2 - mSize.X ,
		 GetSystemMetrics(SM_CYSCREEN) / 2 - mSize.Y };

		const RECT rect = GetAdjustedRect(screenCenter, mSize);

		mHwnd = CreateWindowEx(
			0,                              // Optional window styles.
			mWindowClassName.c_str(),            // Window class
			mName.c_str(),					// Window text
			WS_OVERLAPPEDWINDOW,            // Window style
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr,
			nullptr,
			hInstance,
			userData
		);

		if (mHwnd == nullptr) {
			ZE_FAIL("Win32 window was not created!");
		}
		ShowWindow(mHwnd, SW_SHOW);

		// We cache position manually since event is not triggered on creation
		const RECT screenRect = GetScreenRect();
		OnPositionChanged(screenRect.left, screenRect.top);

		++mWindowIdCounter;
	}
	
	WindowWin32::~WindowWin32()
	{
		SetWindowLongPtr(mHwnd, GWLP_USERDATA, NULL);
		ZE_ASSERT(DestroyWindow(mHwnd), "Unable to destroy Win32 window");
		SetLastError(0);
		UnregisterClass(mWindowClassName.c_str(), nullptr);
		if (GetLastError() > 0) {
			ZE_FAIL("Unable to destroy Win32 window class");
		}
	}

	void WindowWin32::SetVisible()
	{
		ShowWindow(mHwnd, SW_SHOW);
	}

	void WindowWin32::SetHidden()
	{
		ShowWindow(mHwnd, SW_HIDE);
	}

	void WindowWin32::SetName(const std::string& name)
	{
		mName = name;
		SetWindowText(mHwnd, mName.c_str());
	}

	void WindowWin32::SetAspectRatio(int numerator, int denominator)
	{
		if (!IsFullScreen()) {
			ZE_FAIL("WindowWin32::SetAspectRatio Not implemented!");
		}
	}

	void WindowWin32::SetSize(int width, int height)
	{
		if (!IsFullScreen()) {
			const RECT rect = GetAdjustedRect(mPos, {width, height});
			SetWindowPos(mHwnd, HWND_TOP, 
				rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 
				SWP_NOMOVE | SWP_NOOWNERZORDER);
		}
	}

	void WindowWin32::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight)
	{
		if (minWidth < maxWidth && minHeight < maxHeight) {
			mSizeMin = { minWidth , minHeight };
			mSizeMax = { maxWidth , maxHeight };
			SetSize(mSize.X, mSize.Y);
		}
	}

	void WindowWin32::SetScreenPosition(int posX, int posY)
	{
		if(!IsFullScreen()) {
			const RECT rect = GetAdjustedRect({posX, posY}, mSize);
			SetWindowPos(mHwnd, HWND_TOP, 
				rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top,
				SWP_NOSIZE | SWP_NOOWNERZORDER);
		}
	}

	void WindowWin32::SetClipCursor(bool shouldClip)
	{
		if (shouldClip) {
			RECT rectPtr = { 0 };
			GetWindowRect(mHwnd, &rectPtr);
			ClipCursor(&rectPtr);
		}
		else {
			ClipCursor(nullptr);
		}
	}

	void WindowWin32::_onFullScreenChangedBorderless()
	{
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
			const RECT rect = GetAdjustedRect(mPosPrev, mSizePrev);
			SetWindowLongPtr(mHwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
			SetWindowPos(mHwnd, nullptr, rect.left, rect.top, rect.right - rect.left,rect.bottom - rect.top, SWP_FRAMECHANGED);
		}
	}

	void WindowWin32::_onFullScreenChangedMonitor()
	{
	}

	void* WindowWin32::GetApiHandle() const
	{
		return mHwnd;
	}

	SystemHandle WindowWin32::GetSystemHandle() const
	{
		SystemHandle handle;
		handle.mWindow = mHwnd;
		return handle;
	}

	RECT WindowWin32::GetScreenRect() const
	{
		RECT rect;
		GetClientRect(mHwnd, &rect);
		MapWindowPoints(mHwnd, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
		return rect;
	}

	RECT WindowWin32::GetAdjustedRect(const Vec2i& position, const Vec2i& size) const
	{
		RECT rect;
		rect.left = position.X;
		rect.top = position.Y;
		rect.right = rect.left + size.X;
		rect.bottom = rect.top + size.Y;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		return rect;
	}

	const Vec2i& WindowWin32::GetSizeMax() const
	{
		return mSizeMax;
	}

	const Vec2i& WindowWin32::GetSizeMin() const
	{
		return mSizeMin;
	}

	void WindowWin32::SetMinimized()
	{
		ShowWindow(mHwnd, SW_MINIMIZE);
	}

	void WindowWin32::SetMaximized()
	{
		ShowWindow(mHwnd, SW_MAXIMIZE);
	}

	void WindowWin32::SetRestored()
	{
		ShowWindow(mHwnd, SW_RESTORE);
	}

	void WindowWin32::SetFocused()
	{
		SetFocus(mHwnd);
	}

	void WindowWin32::SetAttention()
	{
		FLASHWINFO fInfo = { 0 };
		fInfo.cbSize = sizeof(fInfo);
		fInfo.hwnd = mHwnd;
		fInfo.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		fInfo.uCount = UINT_MAX;
		fInfo.dwTimeout = 0;
		FlashWindowEx(&fInfo);
	}
}

#endif