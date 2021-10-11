// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Window/Window.h"

#ifndef ZE_WINDOW_WIN32
	typedef void* HWND;
#endif

// TODO: Joystick Control
// TODO: Display Management

namespace Zeron {
	class WindowWin32 final : public Window {
	public:
		WindowWin32(const WindowConfig& config);
		~WindowWin32();

		/* Initialize window dependenclies if not initialized yet */

		void BeginFrame() override;
		void EndFrame() override;

		virtual void SetVisible() override;
		virtual void SetHidden() override;
		
		virtual void SetMinimized() override;
		virtual void SetMaximized() override;
		virtual void SetRestored() override;

		virtual void SetFocused() override;
		virtual void SetAttention() override;

		virtual void SetName(const std::string& name) override;
		virtual void SetAspectRatio(int numerator, int denominator) override;
		virtual void SetSize(int width, int height) override;
		virtual void SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight) override;
		virtual void SetScreenPosition(int posX, int posY) override;
		
		virtual void SetClipCursor(bool shouldClip) override;

		virtual void* GetPlatformHandle() const override;

	private:
		virtual void OnFullScreenChangedBorderless() override;
		virtual void OnFullScreenChangedMonitor() override;

	private:
		HWND mHwnd;

		int mSizeMinX;
		int mSizeMinY;
		int mSizeMaxX;
		int mSizeMaxY;

#if ZE_WINDOW_WIN32
	public:
		LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		KeyCode GetKeyCodeWin32(WPARAM wParam, LPARAM lParam);
		MouseCode GetMouseCodeWin32(WPARAM wParam, LPARAM lParam);
#endif
	};

}

