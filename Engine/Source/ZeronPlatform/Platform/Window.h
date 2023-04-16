// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Common/SystemHandle.h>
#include <Platform/WindowConfig.h>
#include <Platform/SystemEvent.h>

namespace Zeron
{
	class SystemEvent;

	class Window {
	public:
		Window(const WindowConfig& config, WindowAPI api);
		virtual ~Window() = default;

		virtual void Update() {}

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

		virtual void* GetApiHandle() const = 0;
		virtual SystemHandle GetSystemHandle() const = 0;

		void OnSystemEvent(const SystemEvent& evt);
		bool HasSystemEvents() const;
		SystemEvent GetNextSystemEvent();
		void FlushEvents();

		const std::string& GetName() const			{ return mName; }
		unsigned int GetID() const					{ return mID; }
		int GetWidth() const						{ return mSize.X; }
		int GetHeight() const						{ return mSize.Y; }
		const Vec2i& GetSize() const				{ return mSize; }
		const Vec2i& GetSizePrev() const			{ return mSizePrev; }
		const Vec2i& GetPos() const					{ return mPos; }
		const Vec2i& GetPosPrev() const				{ return mPosPrev; }
		bool IsFullScreen() const					{ return mIsFullScreen; }
		bool IsMinimized() const					{ return mIsMinimized; }
		bool IsMaximized() const					{ return mIsMaximized; }
		bool IsFocused() const						{ return mIsFocused; }
		bool IsHovered() const						{ return mIsHovered; }
		bool IsHidden() const						{ return mIsHidden; }
		bool IsResizing() const						{ return mIsResizing; }
		bool IsClosing() const						{ return mIsClosing; }
		WindowAPI GetWindowType() const				{ return mWindowType; }
		FullScreenType GetFullScreenType() const	{ return mFullScreenType; }

		Vec2i GetCenter() const;

	protected:
		void OnSizeChanged(int width, int height);
		void OnPositionChanged(int posX, int posY);
		void OnHoverChanged(bool isHovered);
		void OnFocusChanged(bool isFocused);
		void OnVisibilityChanged(bool isHidden);
		void OnRestored();
		void OnMinimized();
		void OnMaximized();

		virtual void _onFullScreenChangedBorderless() {};
		virtual void _onFullScreenChangedMonitor() {};
	
	protected:
		std::string mName;
		int mID;
		Vec2i mSize;
		Vec2i mSizePrev;
		Vec2i mPos;
		Vec2i mPosPrev;
		bool mIsFullScreen;
		bool mIsMinimized;
		bool mIsMaximized;
		bool mIsFocused;
		bool mIsHovered;
		bool mIsHidden;
		bool mIsResizing;
		bool mIsClosing;
		FullScreenType mFullScreenType;
		WindowAPI mWindowType;

		std::queue<SystemEvent> mEventQueue;
	};

}