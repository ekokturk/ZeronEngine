// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Events/Event.h" 
#include "Input/InputTypes.h"
#include "Window/Window.h"


namespace ZeronEngine { namespace Events{

	
    namespace Key
    {

		DECLARE_BASE_EVENT_TYPE(Press)
		{
			Press(const KeyCode & key, const ModifierKeys & modifiers, const WindowHandle & handle = WindowHandle())
				: Key(key), Modifiers(modifiers), ContextHandle(handle) {}

			const KeyCode Key;
			const ModifierKeys Modifiers;
			const WindowHandle ContextHandle;
		};

		DECLARE_BASE_EVENT_TYPE(Release)
		{
			Release(const KeyCode & key, const ModifierKeys & modifiers, const WindowHandle & handle = WindowHandle())
				: Key(key), Modifiers(modifiers), ContextHandle(handle) {}

			const KeyCode Key;
			const ModifierKeys Modifiers;
			const WindowHandle ContextHandle;
		};


		DECLARE_BASE_EVENT_TYPE(Character)
		{
			Character(unsigned int utf8, const ModifierKeys & modifiers, const WindowHandle & handle = WindowHandle())
				: UTF8(utf8), Modifiers(modifiers), ContextHandle(handle) {}

			const unsigned int UTF8;
			const ModifierKeys Modifiers;
			const WindowHandle ContextHandle;
		};

    }
    
}}
