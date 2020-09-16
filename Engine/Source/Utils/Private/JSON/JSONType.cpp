// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "JSON/JSONType.h"


JSONType JSONType::Null = JSONType{ JSONNull() };

std::string JSONType::ToString() const
{
	switch (Value.index())
	{
		case 0:		// -------- Null
		{
			return "null";
		}
		case 1:		// -------- Object
		{
			const auto& object = Get<JSONObject>();
			std::string content = "{";
			for (auto itr = object.begin(); itr != object.end(); ++itr)
			{
				if (itr != object.begin()) {
					content += ", ";
				}
				
				content += "\"";
				content += (*itr).first;
				content += "\"";
				content += " : ";
				content += (*itr).second.ToString();
			}

			content += "}";
			return content;
		}
		case 2:		// -------- Array
		{
			const auto& arr = Get<JSONArray>();
			std::string content = "[";
			for (auto itr = arr.begin(); itr != arr.end(); ++itr)
			{
				if (itr != arr.begin()) {
					content += ", ";
				}
				content += (*itr).ToString();
			}

			content += "]";
			return content;
		}
		case 3:		// -------- String 
		{
			const std::string& str = Get<std::string>();
			std::string temp = "\"";
			temp += str;
			temp += "\"";
			return temp;
		}
		case 4:		// -------- Integer
		{
			return std::to_string(Get<int>());
		}
		case 5:		// -------- Double
		{
			return std::to_string(Get<double>());
		}
		case 6:		// -------- Bool
		{
			return Get<JSONBool>() ? "true" : "false";
		}
		default:
			return std::string();
	}
	
}
