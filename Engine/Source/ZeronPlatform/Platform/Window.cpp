// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/Window.h>

namespace Zeron
{
	Window::Window(const WindowConfig& config, WindowAPI api)
		: mName(config.mName)
		, mID(config.mID)
		, mSize{ config.mWidth, config.mHeight }
		, mSizePrev(mSize)
		, mPos{ 0,0 }
		, mPosPrev{ mPos.X, mPos.Y }
		, mIsFullScreen(config.mIsFullScreen)
		, mIsMinimized(false)
		, mIsMaximized(false)
		, mIsFocused(false)
		, mIsHovered(false)
		, mIsHidden(false)
		, mIsResizing(false)
		, mIsClosing(false)
		, mFullScreenType(FullScreenType::Borderless)
		, mWindowType(api)
	{
	}

	Vec2i Window::GetCenter() const 
	{
		return { mSize.X / 2, mSize.Y / 2 };
	}
	
	void Window::SetFullScreen(bool isFullScreen)
	{
		mIsFullScreen = isFullScreen;
		switch (mFullScreenType) {
			case Zeron::FullScreenType::Borderless: {
				_onFullScreenChangedBorderless();
			} break;
			case Zeron::FullScreenType::Monitor:
			default: ZE_FAIL("Active FullScreenType is not supported!");
		}
		SetClipCursor(mIsFullScreen);
	}

	void Window::SetFullScreenType(FullScreenType fullScreenType)
	{
		if(fullScreenType == mFullScreenType) {
			return;
		}
		// Disable the existing full screen config first
		const bool wasFullScreen = mIsFullScreen;
		if (wasFullScreen) {
			SetFullScreen(false);
		}
		mFullScreenType = fullScreenType;
		if (wasFullScreen) {
			SetFullScreen(true);
		}
	}

	void Window::OnSystemEvent(const SystemEvent& evt)
	{
		std::visit(Visitor {
			[&](const SystemEvent::WindowMaximized&) {
				OnMaximized();
			},
			[&](const SystemEvent::WindowMinimized&) {
				OnMinimized();
			},
			[&](const SystemEvent::WindowRestored&) {
				OnRestored();
			},
			[&](const SystemEvent::WindowSizeChanged& data) {
				OnSizeChanged(data.mWidth, data.mHeight);
			},
			[&](const SystemEvent::WindowResizeStarted& data) {
				mIsResizing = true;
			},
			[&](const SystemEvent::WindowResized& data) {
				mIsResizing = false;
			},
			[&](const SystemEvent::WindowMoved& data) {
				OnPositionChanged(data.mPosX, data.mPosY);
			},
			[&](const SystemEvent::WindowFocused&) {
				OnFocusChanged(true);
			},
			[&](const SystemEvent::WindowUnfocused&) {
				OnFocusChanged(false);
			},
			[&](const SystemEvent::WindowVisibilityChanged& data) {
				OnVisibilityChanged(data.mIsVisible);
			},
			[&](const SystemEvent::WindowClosed& data) {
				mIsClosing = true;
			},
			[&](const SystemEvent::MouseEnter&) {
				OnHoverChanged(true);
			},
			[&](const SystemEvent::MouseExit&) {
				OnHoverChanged(false);
			},
			[](const auto&){ return; },
		}, evt.GetData());

		// TODO: Not here maybe?
		mEventQueue.emplace(evt);
	}

	bool Window::HasSystemEvents() const
	{
		return !mEventQueue.empty();
	}

	SystemEvent Window::GetNextSystemEvent()
	{
		if (!mEventQueue.empty()) {
			auto front = mEventQueue.front();
			mEventQueue.pop();
			return front;
		}
		return { SystemEvent::Invalid{} };
	}

	void Window::FlushEvents()
	{
		std::queue<SystemEvent> empty;
		std::swap(mEventQueue, empty);
	}

	void Window::OnPositionChanged(int posX, int posY)
	{
		mPosPrev = mPos;
		mPos = { posX, posY };
	}

	void Window::OnHoverChanged(bool isHovered)
	{
		mIsHovered = isHovered;
	}

	void Window::OnFocusChanged(bool isFocused)
	{
		mIsFocused = isFocused;
	}

	void Window::OnVisibilityChanged(bool isHidden)
	{
		mIsHidden = isHidden;
	}

	void Window::OnRestored()
	{
		mIsMinimized = false;
		mIsMaximized = false;
	}

	void Window::OnMinimized()
	{
		mIsMinimized = true;
		mIsMaximized = false;
	}

	void Window::OnMaximized()
	{
		mIsMinimized = false;
		mIsMaximized = true;
	}

	void Window::OnSizeChanged(int width, int height)
	{
		mSizePrev = mSize;
		mSize = { width, height };
	}
}

