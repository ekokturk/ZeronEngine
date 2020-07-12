// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Events/Event.h" 
#include "Window/WindowContext.h"

namespace ZeronEngine { namespace Events{
   
    namespace Window
    {
		/* Specified window  is resized */
		DECLARE_EVENT_TYPE(Resize)
		{
			Resize(int width, int height) : Width(width), Height(height) {}
			const int Width;
			const int Height;
		};

		
		/* Specified window  is closed */
		DECLARE_EVENT_TYPE(Close)
		{
			Close(WindowContextHandle handle):ContextHandle(handle){}

			const WindowContextHandle ContextHandle;
		};

		/* All windows are are closed */
		DECLARE_EVENT_TYPE(CloseAll) { };

    	/* Specified window got focused */
		DECLARE_EVENT_TYPE(Focus) { };

		/* Specified window got unfocused */
		DECLARE_EVENT_TYPE(Unfocus) { };

		/* Specified window got minimized */
		DECLARE_EVENT_TYPE(Minimize) { };

		/* Specified window got maximized */
		DECLARE_EVENT_TYPE(Maximize) { };

		/* Specified window got restored from minimized or maximized */
		DECLARE_EVENT_TYPE(Restore) { };

    }
    
}}