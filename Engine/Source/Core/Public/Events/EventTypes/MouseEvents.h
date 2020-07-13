// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Events/Event.h"
#include "Input/InputTypes/InputTypes.h"
#include "Window/WindowContext.h"

namespace ZeronEngine { namespace Events{

    namespace Mouse
    {

		/*
		* Mouse pressed on the window
		* button: Mouse button id from MouseCode enum
		* modifiers: Modifiers keys that are pressed with this event
		* handle: Context handle for window context that mouse exited
		*/
		DECLARE_BASE_EVENT_TYPE(Press)
		{
			Press(const MouseCode&  button, const ModifierKeys& modifiers, const WindowContextHandle& handle = WindowContextHandle())
				: Button(button), Modifiers(modifiers), ContextHandle(handle) {}
			
			const MouseCode Button;
			const ModifierKeys Modifiers;
			const WindowContextHandle ContextHandle;

		};

		DECLARE_BASE_EVENT_TYPE(Release)
		{
			Release(const MouseCode& button, const ModifierKeys& modifiers, const WindowContextHandle& handle = WindowContextHandle())
				: Button(button), ContextHandle(handle) {}

			const MouseCode Button;
			const ModifierKeys Modifiers;
			const WindowContextHandle ContextHandle;
		};

		/*
		* Mouse exited window event
		* ContextHandle: Handle for window context that mouse exited
		*/
		DECLARE_BASE_EVENT_TYPE(Scroll)
		{
			Scroll(float offsetX, float offsetY):
				OffsetX(offsetX), OffsetY(offsetY) {}
				

			const float OffsetX;
			const float OffsetY;
		};

		/*
		 * Mouse moved on window event
		 * PosX: Window position of the mouse in x axis
		 * PosY: Window position of the mouse in y axis
		 */
		DECLARE_BASE_EVENT_TYPE(Move)
		{
			Move(float posX, float posY) :PosX(posX), PosY(posY) {}

			const float PosX;
			const float PosY;
		};

		/*
		* Mouse entered window event
		* ContextHandle: Handle for window context that mouse entered
		*/
		DECLARE_BASE_EVENT_TYPE(Enter)
		{
			Enter(const WindowContextHandle & handle) : ContextHandle(handle) {}

			const WindowContextHandle ContextHandle;
		};

		/*
		* Mouse exited window event
		* ContextHandle: Handle for window context that mouse exited
		*/
		DECLARE_BASE_EVENT_TYPE(Exit)
		{
			Exit(const WindowContextHandle & handle) : ContextHandle(handle) {}

			const WindowContextHandle ContextHandle;
		};
    }
    
}}