// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Events/EventDispatcher.h"
#include "Events/IEventHandler.h"

namespace ZeronEngine
{
	void EventDispatcher::Register(IEventHandler* eventHandler)
	{
		m_EventHandlers.push_back(eventHandler);
	}

	void EventDispatcher::Dispatch(Event& event)
	{
		for(IEventHandler * eventHandler : m_EventHandlers)
		{
			eventHandler->OnEvent(event);
		}
	}
}
