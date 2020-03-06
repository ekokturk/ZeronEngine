// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Events/EventTypes.h"

ZeronEngine::EventType::Custom::Custom(std::string typeName)
	:m_TypeName(typeName)
{
	if (s_TypeMap.find(m_TypeName) == s_TypeMap.end())
	{
		const std::hash<std::string> hasher;
		s_TypeMap.insert(std::make_pair<std::string, size_t>(typeName.c_str(), hasher(typeName)));
	}
}
