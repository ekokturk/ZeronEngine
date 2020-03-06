// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include <string>
#include <unordered_map>

namespace ZeronEngine
{
	namespace EventType
	{
		/// ================ WINDOW EVENTS ===================
		
		struct WindowResize
		{
			
		};
		
		struct WindowClosed
		{
			
		};

		/// ================ KEY EVENTS ===================
		/// 
		struct KeyPressed
		{
			
		};

		struct KeyReleased
		{
			
		};

		/// ================ MOUSE EVENTS ===================

		struct MousePressed
		{
			
		};

		struct MouseReleased
		{
			
		};

		struct MouseMoved
		{

		};

		struct MouseScrolled
		{
			
		};
		
		/// ================ CUSTOM EVENT ===================
		// TODO Add option to pass along data with event Custom(string, ...)

		// Custom event type to take in string as an event type
		class Custom
		{
			static std::unordered_map<std::string, size_t> s_TypeMap;
			std::string m_TypeName;

		public:
			Custom(std::string typeName);
			inline std::string GetName() const { return m_TypeName;  }
			inline size_t GetID() const { return s_TypeMap[m_TypeName]; }
			static size_t GetTypeID(std::string typeName) { return s_TypeMap[typeName]; }
		};

		inline std::unordered_map<std::string, size_t> ZeronEngine::EventType::Custom::s_TypeMap;

#define HANDLE_CUSTOM_EVENT(DataPtr, CustomEventName, Function)\
				if(ZeronEngine::EventType::Custom::GetTypeID((CustomEventName)) == (DataPtr)->GetID())\
				{\
					Function;\
				}\

	}
}

