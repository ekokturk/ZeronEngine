// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once
#include <functional>
#include <variant>
#include "EventTypes.h"

namespace ZeronEngine
{
	// Event Type variant
	using EventData = std::variant<
		EventType::Message,
		EventType::WindowClosed
	>;

	// Event object
	class Event
	{
	private:
		
	public:
		EventData m_Data;
		Event(EventData data);
		~Event();;


	};
}
