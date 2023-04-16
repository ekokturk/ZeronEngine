// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Input/KeyCode.h>
#include <Input/MouseCode.h>

namespace Zeron
{
	class Window;

	class SystemEvent {
	  public:
		struct Context {
			Window* mWindow = nullptr;
		};

		// ----- Window Events -----
		struct WindowCreated {};
		struct WindowClosed {};
		struct WindowFocused {};
		struct WindowUnfocused {};
		struct WindowSizeChanged {
			const int mWidth = 0;
			const int mHeight = 0;
		};
		struct WindowResizeStarted {
			const int mWidth = 0;
			const int mHeight = 0;
		};
		struct WindowResized {
			const int mWidth = 0;
			const int mHeight = 0;
		};
		struct WindowMoved {
			const int mPosX = 0;
			const int mPosY = 0;
		};
		struct WindowMinimized {};
		struct WindowMaximized {};
		struct WindowRestored {};
		struct WindowVisibilityChanged {
			const bool mIsVisible = false;
		};

		// ----- Key Events -----
		struct KeyDown {
			KeyCode mCode;
		};
		struct KeyUp {
			KeyCode mCode;
		};
		struct TextChar {
			const unsigned char mCharacter = 0;
		};

		// ----- Mouse Events -----
		struct MouseEnter {};
		struct MouseExit {};
		struct MouseButtonDown {
			const MouseCode mCode;
		};
		struct MouseButtonUp {
			const MouseCode mCode;
		};
		struct MouseScroll {
			const float mOffsetX = 0.f;
			const float mOffsetY = 0.f;
		};
		struct MouseMoved {
			const int mPosX = 0;
			const int mPosY = 0;
		};

		struct Invalid {};

		using Types = std::variant<
			WindowCreated, WindowClosed, WindowFocused, WindowUnfocused, WindowSizeChanged, WindowResizeStarted, WindowResized, WindowMoved, WindowMinimized, WindowMaximized,
			WindowRestored, WindowVisibilityChanged, KeyDown, KeyUp, TextChar, MouseEnter, MouseExit, MouseButtonDown, MouseButtonUp, MouseScroll, MouseMoved, Invalid>;

		SystemEvent() = default;
		SystemEvent(Types data)
			: mData(data)
		{}

		const Types& GetData() const { return mData; };

	  private:
		Types mData = Invalid{};
	};
}