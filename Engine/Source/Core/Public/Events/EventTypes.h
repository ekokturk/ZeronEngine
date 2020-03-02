// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include <string>

namespace ZeronEngine
{
	namespace EventType
	{

		struct WindowClosed
		{
			WindowClosed(){};
		};

		// Custom event type to take in string as an event type
		class Custom
		{
			static std::unordered_map<std::string, unsigned int> s_TypeMap;
			std::string m_TypeName;

			// TODO Add option to pass along data with event Custom(string, ...)

		public:
			Custom(std::string typeName)
				:m_TypeName(typeName)
			{
				if(s_TypeMap.find(m_TypeName) == s_TypeMap.end())
				{
					s_TypeMap.insert(std::make_pair<const char *, unsigned int>(typeName.c_str(), 3));
				}
			}

			std::string GetName() const { return m_TypeName;  }

			unsigned int GetID() const { return s_TypeMap[m_TypeName]; }

			static unsigned int GetTypeID(std::string typeName) { return s_TypeMap[typeName]; }
		};

#define HANDLE_CUSTOM_EVENT(DataPtr, CustomEventName, Function)\
				if(ZeronEngine::EventType::Custom::GetTypeID((CustomEventName)) == (DataPtr)->GetID())\
				{\
					Function;\
				}\

	}
}

