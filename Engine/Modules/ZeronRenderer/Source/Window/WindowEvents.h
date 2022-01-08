// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include "Window/Input/KeyCode.h"
#include "Window/Input/MouseCode.h"

namespace Zeron {

	class Window;
	
	enum class WindowEventID : uint8_t {
		Invalid = 0,

		WindowClosed,
		WindowCreated,
		WindowFocused,
		WindowUnfocused,
		WindowResized,
		WindowMoved,
		WindowMinimized,
		WindowMaximized,
		WindowRestored,

		KeyDown,
		KeyUp,
		TextChar,

		MouseEnter,
		MouseExit,
		MouseButtonDown,
		MouseButtonUp,
		MouseScroll,
		MouseMoved
	};

	struct WindowEvent
	{
		WindowEvent();
		virtual ~WindowEvent() = default;
		WindowEvent(WindowEvent&& other) = default;
		WindowEvent& operator=(WindowEvent&& other) = default;

		WindowEventID GetID() const { return mID; };
		const char* GetEventName() const;

	protected:
		WindowEvent(WindowEventID type);
		WindowEventID mID;
	};

	// ==================== WINDOW ACTIONS ===============================

	struct WindowEvent_WindowCreated final : public WindowEvent {
		WindowEvent_WindowCreated();
	};

	struct WindowEvent_WindowClosed final : public WindowEvent {
		WindowEvent_WindowClosed();
	};

	struct WindowEvent_WindowFocused final : public WindowEvent {
		WindowEvent_WindowFocused();
	};

	struct WindowEvent_WindowUnfocused final : public WindowEvent {
		WindowEvent_WindowUnfocused();
	};

	struct WindowEvent_WindowMaximized final : public WindowEvent {
		WindowEvent_WindowMaximized();
	};

	struct WindowEvent_WindowMinimized final : public WindowEvent {
		WindowEvent_WindowMinimized();
	};

	struct WindowEvent_WindowRestored final : public WindowEvent {
		WindowEvent_WindowRestored();
	};

	struct WindowEvent_WindowResized final : public WindowEvent {
		WindowEvent_WindowResized(int width, int height);
		const int mWidth;
		const int mHeight;
	};

	struct WindowEvent_WindowMoved final : public WindowEvent {
		WindowEvent_WindowMoved(int posX, int posY);
		const int mPosX;
		const int mPosY;
	};

	// ==================== KEY ACTIONS ===============================

	struct WindowEvent_KeyDown final : public WindowEvent {
		WindowEvent_KeyDown(KeyCode key);
		KeyCode mCode;
	};

	struct WindowEvent_KeyUp final : public WindowEvent {
		WindowEvent_KeyUp(KeyCode key);
		KeyCode mCode;
	};

	struct WindowEvent_TextChar final : public WindowEvent {
		WindowEvent_TextChar(unsigned char character);
		const unsigned char mCharacter;
	};

	// ==================== MOUSE ACTIONS ===============================

	struct WindowEvent_MouseEnter final : public WindowEvent {
		WindowEvent_MouseEnter();
	};

	struct WindowEvent_MouseExit final : public WindowEvent {
		WindowEvent_MouseExit();
	};

	struct WindowEvent_MouseDown final : public WindowEvent {
		WindowEvent_MouseDown(MouseCode code);
		const MouseCode mCode;
	};

	struct WindowEvent_MouseUp final : public WindowEvent {
		WindowEvent_MouseUp(MouseCode code);
		const MouseCode mCode;
	};

	struct WindowEvent_MouseMoved final : public WindowEvent {
		WindowEvent_MouseMoved(int posX, int posY);
		const int mPosX;
		const int mPosY;
	};

	struct WindowEvent_MouseScrolled final : public WindowEvent {
		WindowEvent_MouseScrolled(float offsetX, float offsetY);
		const float mOffsetX;
		const float mOffsetY;
	};
}