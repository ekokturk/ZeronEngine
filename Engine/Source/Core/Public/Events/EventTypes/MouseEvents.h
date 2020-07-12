// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Events/Event.h"
#include "Window/WindowContext.h"

namespace ZeronEngine { namespace Events{

    namespace Mouse
    {
		/*
		 * Mouse moved on window event
		 * PosX: Window postion of the mouse in x axis
		 * PosY: Window postion of the mouse in y axis
		 */
		DECLARE_EVENT_TYPE(Move)
		{
			Move(float posX, float posY):PosX(posX), PosY(posY){}
			
			const float PosX;
			const float PosY;
		};

		/*
		* Mouse entered window event
		* ContextHandle: Handle for window context that mouse entered
		*/
		DECLARE_EVENT_TYPE(Enter)
		{
			Enter(WindowContextHandle handle) : ContextHandle(handle) {}

			const WindowContextHandle ContextHandle;
		};

		/*
		* Mouse exited window event
		* ContextHandle: Handle for window context that mouse exited
		*/
		DECLARE_EVENT_TYPE(Exit)
		{
			Exit(WindowContextHandle handle) : ContextHandle(handle) {}

			const WindowContextHandle ContextHandle;
		};
		
		struct Press
		{

		};

		struct Release
		{

		};


		/*
		* Mouse exited window event
		* ContextHandle: Handle for window context that mouse exited
		*/
		DECLARE_EVENT_TYPE(Scroll)
		{
			Scroll(float offsetX, float offsetY):
				OffsetX(offsetX), OffsetY(offsetY) {}
				

			const float OffsetX;
			const float OffsetY;
		};


    }
    
}}