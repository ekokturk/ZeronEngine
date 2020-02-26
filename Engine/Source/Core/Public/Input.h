// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include <functional>
#include "Events/Event.h"

namespace ZeronEngine
{
	class Event;
	
	class Input
	{
	private:
		std::function<void(Event&)> m_EventCallback;

	public:
		
		void SetEventCallback(std::function<void(Event&)>& callback);
	};
}
