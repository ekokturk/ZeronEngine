// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Input/Input.h"

namespace ZeronEngine
{
	
	class EventDispatcher;
	class Input;

	/*
	 * TODO: Input Mapping
	 * TODO: Static Input Bind Unbind
	 */
	
	class InputModule 
	{
		
	public:
		void RegisterEvents(const EventDispatcher& Dispatcher);

		void Init();
		void Update();
		void Destroy();

		
		
		
	private:

		template<typename T>
		void SetupInputClass()
		{
			m_InputClass = std::unique_ptr<Input>(new T);
		}

	private:

		std::unique_ptr<Input> m_InputClass;

		
	};
}
