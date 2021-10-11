// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "WindowEvents.h"

namespace Zeron {
	
	WindowEvent::WindowEvent() 
		: mID(WindowEventID::Invalid)
	{
	}

	const char* WindowEvent::GetEventName() const
	{
		switch (mID) {
		case Zeron::WindowEventID::WindowClosed:		return "Window Closed";
		case Zeron::WindowEventID::WindowCreated:		return "Window Created";
		case Zeron::WindowEventID::WindowFocused:		return "Window Focused";
		case Zeron::WindowEventID::WindowUnfocused:		return "Window Unfocused";
		case Zeron::WindowEventID::WindowResized:		return "Window Resized";
		case Zeron::WindowEventID::WindowMoved:			return "Window Moved";
		case Zeron::WindowEventID::WindowMinimized:		return "Window Minimized";
		case Zeron::WindowEventID::WindowMaximized:		return "Window Maximized";
		case Zeron::WindowEventID::WindowRestored:		return "Window Restored";
		case Zeron::WindowEventID::KeyDown:				return "Key Down";
		case Zeron::WindowEventID::KeyUp:				return "Key Up";
		case Zeron::WindowEventID::TextChar:			return "Text Character";
		case Zeron::WindowEventID::MouseEnter:			return "Mouse Enter";
		case Zeron::WindowEventID::MouseExit:			return "Mouse Exit";
		case Zeron::WindowEventID::MouseButtonDown:		return "Mouse Button Down";
		case Zeron::WindowEventID::MouseButtonUp:		return "Mouse Button Up";
		case Zeron::WindowEventID::MouseScroll:			return "Mouse Scroll";
		case Zeron::WindowEventID::MouseMoved:			return "Mouse Moved";
		default: return "Invalid";
		}
	}

	WindowEvent::WindowEvent(WindowEventID id)
		: mID(id)
	{
	}

	WindowEvent_WindowCreated::WindowEvent_WindowCreated()
		: WindowEvent(WindowEventID::WindowCreated)
	{
	}

	WindowEvent_WindowClosed::WindowEvent_WindowClosed()
		: WindowEvent(WindowEventID::WindowClosed)
	{
	}
	WindowEvent_WindowFocused::WindowEvent_WindowFocused()
		: WindowEvent(WindowEventID::WindowFocused)
	{
	}

	WindowEvent_WindowUnfocused::WindowEvent_WindowUnfocused()
		: WindowEvent(WindowEventID::WindowUnfocused)
	{
	}

	WindowEvent_WindowMaximized::WindowEvent_WindowMaximized()
		: WindowEvent(WindowEventID::WindowMaximized)
	{
	}

	WindowEvent_WindowMinimized::WindowEvent_WindowMinimized()
		: WindowEvent(WindowEventID::WindowMinimized)
	{
	}

	WindowEvent_WindowRestored::WindowEvent_WindowRestored()
		: WindowEvent(WindowEventID::WindowRestored)
	{
	}

	WindowEvent_KeyDown::WindowEvent_KeyDown(KeyCode key)
		: WindowEvent(WindowEventID::KeyDown)
		, mCode(key)
	{
	}

	WindowEvent_KeyUp::WindowEvent_KeyUp(KeyCode key)
		: WindowEvent(WindowEventID::KeyUp)
		, mCode(key)
	{
	}

	WindowEvent_TextChar::WindowEvent_TextChar(unsigned char character)
		: WindowEvent(WindowEventID::TextChar)
		, mCharacter(character)
	{
	}

	WindowEvent_MouseEnter::WindowEvent_MouseEnter()
		: WindowEvent(WindowEventID::MouseEnter)
	{
	}

	WindowEvent_MouseExit::WindowEvent_MouseExit()
		: WindowEvent(WindowEventID::MouseExit)
	{
	}

	WindowEvent_MouseDown::WindowEvent_MouseDown(MouseCode code)
		: WindowEvent(WindowEventID::MouseButtonDown)
		, mCode(code)
	{
	}

	WindowEvent_MouseUp::WindowEvent_MouseUp(MouseCode code)
		: WindowEvent(WindowEventID::MouseButtonUp)
		, mCode(code)
	{
	}

	WindowEvent_MouseMoved::WindowEvent_MouseMoved(int posX, int posY)
		: WindowEvent(WindowEventID::MouseMoved)
		, mPosX(posX)
		, mPosY(posY)
	{
	}
	
	WindowEvent_MouseScrolled::WindowEvent_MouseScrolled(float offsetX, float offsetY)
		: WindowEvent(WindowEventID::MouseScroll)
		, mOffsetX(offsetX)
		, mOffsetY(offsetY)
	{
	}
	

	WindowEvent_WindowResized::WindowEvent_WindowResized(int width, int height)
		: WindowEvent(WindowEventID::WindowResized)
		, mWidth(width)
		, mHeight(height)
	{
	}

	WindowEvent_WindowMoved::WindowEvent_WindowMoved(int posX, int posY)
		: WindowEvent(WindowEventID::WindowMoved)
		, mPosX(posX)
		, mPosY(posY)
	{
	}

}

