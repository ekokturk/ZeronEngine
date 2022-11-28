// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Window/Window.h>

#ifdef ZE_WINDOW_WIN32

// TODO: Joystick Control
// TODO: Display Management

namespace Zeron {
	class WindowWin32 final : public Window {
	public:
		WindowWin32(const WindowConfig& config);
		~WindowWin32();

		bool Init() override;

		void Update() override;

		void SetVisible() override;
		void SetHidden() override;
		
		void SetMinimized() override;
		void SetMaximized() override;
		void SetRestored() override;

		void SetFocused() override;
		void SetAttention() override;

		void SetName(const std::string& name) override;
		void SetAspectRatio(int numerator, int denominator) override;
		void SetSize(int width, int height) override;
		void SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight) override;
		void SetScreenPosition(int posX, int posY) override;
		
		void SetClipCursor(bool shouldClip) override;

		void* GetApiHandle() const override;
		SystemHandle GetSystemHandle() const override;

	private:
		void _onFullScreenChangedBorderless() override;
		void _onFullScreenChangedMonitor() override;

		HWND mHwnd;
		
		bool mIsResizing;

		int mSizeMinX;
		int mSizeMinY;
		int mSizeMaxX;
		int mSizeMaxY;

	public:
		LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		// Get client Rect in screen coordinates
		RECT _getScreenRect() const;
		// Get window Rect from client coordinates
		RECT _getAdjustedRect(const Vec2i& position, const Vec2i& size) const;
		
		KeyCode _getKeyCodeWin32(WPARAM wParam, LPARAM lParam);
		MouseCode _getMouseCodeWin32(WPARAM wParam, LPARAM lParam);

		static uint32_t mWindowIdCounter;
	};

}

#endif
