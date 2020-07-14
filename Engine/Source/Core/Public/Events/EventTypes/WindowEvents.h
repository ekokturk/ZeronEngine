// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Events/Event.h" 
#include "Window/WindowContext.h"

namespace ZeronEngine { namespace Events{
   
    namespace Window
    {
		/* Specified window  is resized */
		DECLARE_BASE_EVENT_TYPE(Resize)
		{
			Resize(int width, int height, WindowContextHandle&& ContextHandle = WindowContextHandle())
			: Width(width), Height(height), ContextHandle(ContextHandle){}
			
			const int Width;
			const int Height;
			const WindowContextHandle ContextHandle;
		};

		
		/* Specified window  is closed */
		DECLARE_BASE_EVENT_TYPE(Close)
		{
			Close(WindowContextHandle handle):ContextHandle(handle){}

			const WindowContextHandle ContextHandle;
		};

		/* Main window is closed */
		DECLARE_BASE_EVENT_TYPE(MainClose) { };

    	/* Specified window got focused */
		DECLARE_BASE_EVENT_TYPE(Focus)
		{
			Focus(WindowContextHandle&& contextHandle = WindowContextHandle())
				:ContextHandle(contextHandle){}
			
			const WindowContextHandle ContextHandle;
		};

		/* Specified window got unfocused */
		DECLARE_BASE_EVENT_TYPE(Unfocus) { };

		/* Specified window got minimized */
		DECLARE_BASE_EVENT_TYPE(Minimize) { };

		/* Specified window got maximized */
		DECLARE_BASE_EVENT_TYPE(Maximize) { };

		/* Specified window got restored from minimized or maximized */
		DECLARE_BASE_EVENT_TYPE(Restore) { };

    }
    
}}