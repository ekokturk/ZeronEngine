// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Window/WindowConfig.h"

namespace Zeron {

	class Window {
	public:
		// This needs to be implemented to create a window for the specific platform
		static std::unique_ptr<Window> CreateWindow(WindowType type, const WindowConfig& config);

		Window(const WindowConfig& config);
		virtual ~Window() = default;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void SetVisible() = 0;
		virtual void SetHidden() = 0;

		virtual void SetMinimized() = 0;
		virtual void SetMaximized() = 0;
		virtual void SetRestored() = 0;

		virtual void SetFocused() = 0;
		virtual void SetAttention() = 0;

		virtual void SetName(const std::string& name) = 0;

		virtual void SetSize(int width, int height) = 0;
		virtual void SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight) = 0;
		virtual void SetAspectRatio(int numerator, int denominator) = 0;
		virtual void SetScreenPosition(int posX, int posY) = 0;
		virtual void SetFullScreen(bool isFullScreen) = 0;

		virtual void* GetPlatformHandle() const = 0;

		[[nodiscard]] const std::string& GetName() const	{ return mName; }
		[[nodiscard]] unsigned int GetID() const			{ return mID; }
		[[nodiscard]] int GetWidth() const					{ return mWidth; }
		[[nodiscard]] int GetHeight() const					{ return mHeight; }
		[[nodiscard]] bool IsFullScreen() const				{ return mIsFullScreen; }
		[[nodiscard]] bool IsMinimized() const				{ return mIsMinimized; }
		[[nodiscard]] WindowType GetWindowType() const		{ return mWindowType; }

	protected:

		std::string mName;
		int mID;
		
		int mWidth;
		int mHeight;
		int mWidthPrev;
		int mHeightPrev;

		int mScreenPosX;
		int mScreenPosY;
		int mScreenPrevPosX;
		int mScreenPrevPosY;

		bool mIsFullScreen;
		bool mIsMinimized;
		WindowType mWindowType;
	};

}