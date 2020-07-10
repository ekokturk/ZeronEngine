// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Events/Event.h" 
#include "Window/WindowContext.h"

namespace ZeronEngine
{
	namespace Events
	{
		/// ================ WINDOW EVENTS ===================

		/* Specific Window is resized */
		DECLARE_EVENT_TYPE(Window_Resize)
		{
			Window_Resize() : sizeX(0) {}
			const int sizeX;
		};

		
		/* Specific window is closed */
		DECLARE_EVENT_TYPE(Window_Closed)
		{
			Window_Closed(WindowContextHandle context):ContextHandle(context){}

			const WindowContextHandle ContextHandle;
		};

		/* All windows are are closed */
		DECLARE_EVENT_TYPE(Window_All_Closed) { };
		
		/// ================ KEY EVENTS ===================
/// 
		struct KeyPressed
		{

		};

		struct KeyReleased
		{

		};

		/// ================ MOUSE EVENTS ===================

		DECLARE_EVENT_TYPE(Mouse_Moved)
		{
			Mouse_Moved(float posX, float posY):PosX(posX), PosY(posY){}
			
			const float PosX;
			const float PosY;
		};
		
		struct MousePressed
		{

		};

		struct MouseReleased
		{

		};



		struct MouseScrolled
		{

		};

	}
		
		

		


		
}

