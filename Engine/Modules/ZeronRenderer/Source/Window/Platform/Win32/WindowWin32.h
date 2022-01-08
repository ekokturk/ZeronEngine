// Copyright (C) Eser Kokturk. All Rights Reserved.

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

		virtual bool Init() override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

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

		virtual void* GetAPIHandle() const override;
		virtual void* GetPlatformHandle() const override;

	private:
		virtual void OnFullScreenChangedBorderless_() override;
		virtual void OnFullScreenChangedMonitor_() override;

	private:
		HWND mHwnd;
		
		bool mIsResizing;

		int mSizeMinX;
		int mSizeMinY;
		int mSizeMaxX;
		int mSizeMaxY;

#if ZE_WINDOW_WIN32
	public:
		LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		// Get client Rect in screen coordinates
		RECT GetScreenRect_() const;
		// Get window Rect from client coordinates
		RECT GetAdjustedRect_(const Vec2i& position, const Vec2i& size) const;
		
		KeyCode GetKeyCodeWin32_(WPARAM wParam, LPARAM lParam);
		MouseCode GetMouseCodeWin32_(WPARAM wParam, LPARAM lParam);
#endif
	};

}

