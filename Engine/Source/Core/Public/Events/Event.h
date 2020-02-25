// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once
#include <functional>

namespace ZeronEngine
{
	enum class EEventType
	{
		AppStart,
		AppStop
	};

	
	class Event
	{
	private:
		
		
	public:
		EEventType m_Type;
	};
}
