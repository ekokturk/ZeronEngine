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
		struct Custom
		{
			std::string TypeName;
			unsigned int TypeID;

			// TODO Add option to pass along data with event Custom(string, ...)

			Custom(std::string typeName)
				:TypeName(typeName),
				 TypeID(0) // TODO Create string id for events
			{
			}
			
		};

	}
}

