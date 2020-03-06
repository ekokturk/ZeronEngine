// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once

#include "Events/Event.h"

namespace ZeronEngine
{
	class IEventHandler;
	
	class EventDispatcher
	{
		
	private:
		std::vector<IEventHandler*> m_EventHandlers;
		
	public:
		void Register(IEventHandler* eventHandler);
		void Dispatch(Event& event);
	};


}

