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

		struct Message
		{
			std::string Data;
		};

	}
}

