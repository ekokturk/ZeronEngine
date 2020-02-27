// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once

namespace ZeronEngine
{
	class Event;
	
	class IEventHandler
	{

	public:
		virtual void OnEvent(const Event& e) = 0;
	protected:
		virtual ~IEventHandler() {}
	};

}
