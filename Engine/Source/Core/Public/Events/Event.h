// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once
#include <functional>
#include <variant>
#include "EventTypes.h"

namespace ZeronEngine
{
	// Event Type variant
	using EventData = std::variant<
		EventType::Custom,
		EventType::WindowClosed
	>;

	// Event object
	class Event
	{
	private:
		EventData m_Data;

	public:
		Event(EventData data);
		~Event();

		// Get reference to event data, returning in conditional statement will check for event type
		template<typename T>
		auto GetDataRef() const { return std::get_if<T>(&m_Data); }
		
	};
}
