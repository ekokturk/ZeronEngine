// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once
#include "Hash.h"

#define DECLARE_BASE_EVENT_TYPE(EventClass)\
	struct EventClass : EventBase<EventClass>\

#define DECLARE_EVENT_TYPE(EventClass, EventParentClass)\
	struct EventClass : EventParentClass\



namespace ZeronEngine
{
	namespace Events
	{
		/*
		 * Base class for all event types to inherit from
		 * Event dispatcher will expect events that are derived from EventBase
		 * If event is consumed it will no longer be available for dispatch
		 */
		template<typename ClassName>
		struct EventBase
		{
			// TODO: make this only template name (typeid is ugly)
			// unique identifier name for event
			constexpr static std::string GetName() { return typeid(ClassName).name(); }
			constexpr static size_t GetID() { return Hash<const char*>(typeid(ClassName).name()); }

			bool operator==(const EventBase& other) const
			{
				return (other.GetID() == GetID());
			}

			// Check if event is already consumed
			bool IsConsumed() const { return m_isConsumed; }

			// Consume the event so it will not be called again
			void Consume() const { m_isConsumed = true; };

		protected:
			EventBase() :m_isConsumed(false) {};

			// Determine if event is consumed or not (consumed events will not be fired again by dispatcher)
			mutable bool m_isConsumed; 
		};
	}
}


