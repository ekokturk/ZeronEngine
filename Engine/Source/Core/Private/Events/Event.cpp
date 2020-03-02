// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Events/Event.h"
#include "Events/EventTypes.h"

std::unordered_map<std::string, unsigned int> ZeronEngine::EventType::Custom::s_TypeMap;

ZeronEngine::Event::Event(EventData data)
	:m_Data(data)
{
}

ZeronEngine::Event::~Event()
{
}
