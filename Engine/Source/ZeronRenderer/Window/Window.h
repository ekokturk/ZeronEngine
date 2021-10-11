// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Window/WindowConfig.h"
#include "Window/WindowEvents.h"

namespace Zeron {

	class Window {
	public:
		static std::unique_ptr<Window> CreatePlatformWindow(WindowAPI type, const WindowConfig& config);

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
		
		virtual void SetClipCursor(bool shouldClip) = 0;

		void SetFullScreen(bool isFullScreen);
		void SetFullScreenType(FullScreenType fullScreenType);

		virtual void* GetPlatformHandle() const = 0;

		std::unique_ptr<WindowEvent> GetNextEvent();

		[[nodiscard]] const std::string& GetName() const		{ return mName; }
		[[nodiscard]] unsigned int GetID() const				{ return mID; }
		[[nodiscard]] int GetWidth() const						{ return mWidth; }
		[[nodiscard]] int GetHeight() const						{ return mHeight; }
		[[nodiscard]] bool IsFullScreen() const					{ return mIsFullScreen; }
		[[nodiscard]] bool IsMinimized() const					{ return mIsMinimized; }
		[[nodiscard]] bool IsMaximized() const					{ return mIsMaximized; }
		[[nodiscard]] bool IsFocused() const					{ return mIsFocused; }
		[[nodiscard]] bool IsHovered() const					{ return mIsHovered; }
		[[nodiscard]] bool IsHidden() const						{ return mIsHidden; }
		[[nodiscard]] WindowAPI GetWindowType() const			{ return mWindowType; }
		[[nodiscard]] FullScreenType GetFullScreenType() const	{ return mFullScreenType; }

		// TODO: Implement these with data structures
		//[[nodiscard]] Vec2I GetCenter() const;
		//[[nodiscard]] Rect GetRect() const;

	protected:
		void ClearEventQueue();
		void OnSizeChanged(int width, int height);
		void OnPositionChanged(int posX, int posY);
		void OnHoverChanged(bool isHovered);
		void OnFocusChanged(bool isFocused);
		void OnVisibilityChanged(bool isHidden);
		void OnRestored();
		void OnMinimized();
		void OnMaximized();

		virtual void OnFullScreenChangedBorderless() = 0;
		virtual void OnFullScreenChangedMonitor() = 0;
	
	protected:
		std::string mName;
		int mID;
		
		int mWidth;
		int mHeight;
		int mWidthPrev;
		int mHeightPrev;

		int mPosX;
		int mPosY;
		int mPosPrevX;
		int mPosPrevY;

		// TODO: Distinguish between Borderless Window vs Full Screen
		bool mIsFullScreen;
		bool mIsMinimized;
		bool mIsMaximized;
		bool mIsFocused;
		bool mIsHovered;
		bool mIsHidden;
		WindowAPI mWindowType;
		FullScreenType mFullScreenType;

		std::queue<std::unique_ptr<WindowEvent>> mEventQueue;
	};

}