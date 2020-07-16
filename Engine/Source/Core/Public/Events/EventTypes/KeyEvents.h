// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Events/Event.h" 
#include "Input/InputTypes.h"
#include "Window/WindowContext.h"


namespace ZeronEngine { namespace Events{

	
    namespace Key
    {

		DECLARE_BASE_EVENT_TYPE(Press)
		{
			Press(const KeyCode & key, const ModifierKeys & modifiers, const WindowContextHandle & handle = WindowContextHandle())
				: Key(key), Modifiers(modifiers), ContextHandle(handle) {}

			const KeyCode Key;
			const ModifierKeys Modifiers;
			const WindowContextHandle ContextHandle;
		};

		DECLARE_BASE_EVENT_TYPE(Release)
		{
			Release(const KeyCode & key, const ModifierKeys & modifiers, const WindowContextHandle & handle = WindowContextHandle())
				: Key(key), Modifiers(modifiers), ContextHandle(handle) {}

			const KeyCode Key;
			const ModifierKeys Modifiers;
			const WindowContextHandle ContextHandle;
		};

		DECLARE_BASE_EVENT_TYPE(Repeat)
		{
			Repeat(const KeyCode & key, const ModifierKeys & modifiers, const WindowContextHandle & handle = WindowContextHandle())
				: Key(key), Modifiers(modifiers), ContextHandle(handle) {}

			const KeyCode Key;
			const ModifierKeys Modifiers;
			const WindowContextHandle ContextHandle;
		};


		DECLARE_BASE_EVENT_TYPE(Character)
		{
			Character(unsigned int utf8, const ModifierKeys & modifiers, const WindowContextHandle & handle = WindowContextHandle())
				: UTF8(utf8), Modifiers(modifiers), ContextHandle(handle) {}

			const unsigned int UTF8;
			const ModifierKeys Modifiers;
			const WindowContextHandle ContextHandle;
		};

    }
    
}}
