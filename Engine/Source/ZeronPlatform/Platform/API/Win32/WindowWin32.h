// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/Window.h>

#ifdef ZE_WINDOW_WIN32

// TODO: Joystick Control
// TODO: Display Management

namespace Zeron {
	class SystemEventProcessor;

	class WindowWin32 final : public Window {
	public:
		WindowWin32(const WindowConfig& config, WNDPROC wndProc, void* userData = nullptr);
		~WindowWin32();

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

		// Get client Rect in screen coordinates
		RECT GetScreenRect() const;
		// Get window Rect from client coordinates
		RECT GetAdjustedRect(const Vec2i& position, const Vec2i& size) const;

		const Vec2i& GetSizeMax() const;
		const Vec2i& GetSizeMin() const;

	private:
		void _onFullScreenChangedBorderless() override;
		void _onFullScreenChangedMonitor() override;

		HWND mHwnd;
		std::string mWindowClassName;
		Vec2i mSizeMin;
		Vec2i mSizeMax;
		static uint32_t mWindowIdCounter;
	};

}

#endif
