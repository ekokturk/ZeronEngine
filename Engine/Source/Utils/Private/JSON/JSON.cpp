// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "JSON/JSON.h"

JSON JSON::Parse(const std::string& content)
{
	std::string temp;
	auto currItr = content.begin();
	auto endItr = content.end();
	JSONType value(JSONType::NullType());
	Parse_Internal(value, currItr, endItr);
	return JSON{ value };
}

std::string JSON::Stringify(const JSON& InJson, bool isExpanded)
{
	return InJson.Data.ToString();
}

bool JSON::ExportToFile(const JSON& json, const std::string& filePath)
{
	std::ofstream file(filePath);
	if (file.is_open())
	{
		file << json.Data.ToString();
		return true;
	}

	return false;
}

bool JSON::ImportFromFile(JSON& json, const std::string& filePath)
{
	std::ifstream file(filePath);
	if (file.is_open())
	{
		const std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		json = Parse(content);
		return true;
	}

	return false;
}

std::string JSON::ToString() const
{
	return Data.ToString();
}

bool JSON::Parse_Internal(JSONType& value, std::string::const_iterator& currItr, std::string::const_iterator& endItr)
{
	// Convert all consecutive digits into whole number
	for (; currItr != endItr; ++currItr)
	{
		if (ParseValue(value, currItr, endItr) == false)
		{
			return false;
		}
	}
	return true;
}

bool JSON::ParseValue(JSONType& value, std::string::const_iterator& currItr, std::string::const_iterator& endItr)
{

	for (; currItr != endItr; ++currItr)
	{
		switch (*currItr)
		{
		case '{':	// ------------ JSON Object
		{
			JSONObject obj;
			if (ParseObject(obj, currItr, endItr)) {
				value = std::move(obj);
				return true;
			}
			return false;
		}
		case '[':	// ------------ JSON Array
		{
			JSONArray arr;
			if (ParseArray(arr, currItr, endItr)) {
				value = std::move(arr);
				return true;
			}
			return false;
		}
		case '"':	// ------------ String
		{
			std::string str;
			if (ParseString(str, currItr, endItr)) {
				value = std::move(str);
				return true;
			}
			return false;
		}
		case 'n': // ------------ Null
		{
			const std::string check("null");
			for (auto checkItr = check.begin(); checkItr != check.end(); ++checkItr)
			{
				if (currItr == endItr || *checkItr != *currItr) {
					return false;
				} ++currItr;
			} --currItr;
			value = JSONType::NullType();
			return true;
		}
		case 't': // ------------ Boolean True
		{
			const std::string check("true");
			for (auto checkItr = check.begin(); checkItr != check.end(); ++checkItr)
			{
				if (currItr == endItr || *checkItr != *currItr) {
					return false;
				} ++currItr;
			} --currItr;
			value = JSONBool{ true };
			return true;
		}
		case 'f': // ------------ Boolean False
		{
			const std::string check("false");
			for (auto checkItr = check.begin(); checkItr != check.end(); ++checkItr)
			{
				if (currItr == endItr || *checkItr != *currItr) {
					return false;
				} ++currItr;
			} --currItr;
			value = JSONBool{ false };
			return true;
		}
		default:
		{
			if (isdigit(*currItr) || *currItr == '-')
			{
				return ParseNumber(value, currItr, endItr);
			}

			if (*currItr != ' ' && *currItr != '\n' && *currItr != '\t')
			{
				return false;
			}
		};
		}

	}
	value = JSONType::NullType();
	return false;
}

bool JSON::ParseNumber(JSONType& value, std::string::const_iterator& currItr, std::string::const_iterator& endItr)
{
	WholeNumber fraction;
	WholeNumber exponent;
	const WholeNumber integer = ParseWholeNumber(currItr, endItr);

	for (++currItr; currItr != endItr; ++currItr)
	{
		if (*currItr == 'E' || *currItr == 'e')
		{
			++currItr;
			exponent = ParseWholeNumber(currItr, endItr);
			if (exponent.DigitCount == 0)
			{
				return false;
			}
		}
		else if (*currItr == '.')
		{
			++currItr;
			fraction = ParseWholeNumber(currItr, endItr);
			if (fraction.DigitCount == 0 || fraction.Value < 0)
			{
				return false;
			}
		}
		else {
			break;
		}
	}

	--currItr;

	if (fraction.DigitCount == 0)
	{
		value = static_cast<int>(integer.Value * pow(10, exponent.Value));
		return true;
	}
	else if (fraction.DigitCount > 0)
	{
		const double fractionValue = ((integer.Value < 0) ? -1 : (integer.Value > 0)) * fraction.Value * pow(10, -fraction.DigitCount);
		value = (integer.Value + fractionValue) * pow(10, exponent.Value);
		return true;
	}

	return false;
}

JSON::WholeNumber JSON::ParseWholeNumber(std::string::const_iterator& currItr, std::string::const_iterator& endItr)
{
	WholeNumber number;
	int sign = 1;

	// Check if negative
	if (*currItr == '-' && currItr != endItr)
	{
		sign = -1;
		++currItr;
	}

	// Convert all consecutive digits into whole number
	for (; currItr != endItr; ++currItr)
	{
		if (isdigit(*currItr))
		{
			number.Value = number.Value * 10 + (*currItr - '0');
			number.DigitCount++;
		}
		else
		{
			break;
		}
	}
	--currItr;

	number.Value *= sign;

	return number;
}


bool JSON::ParseString(std::string& str, std::string::const_iterator& currItr, std::string::const_iterator& endItr)
{
	const auto begin = currItr + 1;
	for (++currItr; currItr != endItr; ++currItr)
	{
		if ('"' == *currItr)	// Expect end of string
		{
			str = std::string(begin, currItr);
			return true;
		}
	}

	return false;
}

bool JSON::ParseObject(JSONObject& jObject, std::string::const_iterator& currItr, std::string::const_iterator& endItr)
{
	auto expectedType = ExpectedObjectType::Key;	// Flag to check if object is valid
	std::string key;								// Container for the current key

	const auto begin = currItr + 1;
	for (++currItr; currItr != endItr; ++currItr)
	{
		switch (*currItr)
		{
		case '}':	// ----------- Next Pair 
		{
			// Return true if nothing is expected as the next type
			return expectedType == ExpectedObjectType::None;
		}
		case ',':	// ----------- Next Value 
		{
			// Check if value is already registered and separator is required
			if (expectedType != ExpectedObjectType::None) {
				return false;
			}
			expectedType = ExpectedObjectType::Key;
			break;
		}
		case ':':	// ----------- Current Value
		{
			// Check if key is already set and separator is required
			if (expectedType != ExpectedObjectType::Separator) {
				return false;
			}
			expectedType = ExpectedObjectType::Value;
			break;
		}
		case '"':	// ----------- Current Pair
		{
			// We intentionally skip to default case if we are expecting a value
			if (expectedType != ExpectedObjectType::Value) {
				// Check if key was the expected type and it is parsed correctly
				if (expectedType != ExpectedObjectType::Key || ParseString(key, currItr, endItr) == false) {
					return false;
				}
				expectedType = ExpectedObjectType::Separator;
				break;
			}
		}
		default:
		{
			if (expectedType == ExpectedObjectType::Value) {
				JSONType value;
				if (ParseValue(value, currItr, endItr) == false) {
					return false;
				}
				jObject.emplace(key, value);
				key = "";
				expectedType = ExpectedObjectType::None;
			}
			else if (*currItr != ' ' && *currItr != '\n' && *currItr != '\t')
			{
				return false;
			}
		};
		}
	}

	return false;
}

bool JSON::ParseArray(JSONArray& jArray, std::string::const_iterator& currItr, std::string::const_iterator& endItr)
{
	bool expectValue = true;	// Expect next item in the array to be a value

	const auto begin = currItr + 1;
	for (++currItr; currItr != endItr; ++currItr)
	{
		switch (*currItr)
		{
		case ']':	// ----------- Next Pair 
		{
			return expectValue == false;
		}
		case ',':	// ----------- Next Value 
		{
			if (expectValue || currItr == endItr) {
				return false;
			}
			expectValue = true;
			break;
		}
		default:
		{

			if (expectValue) {
				JSONType item;
				if (ParseValue(item, currItr, endItr) == false) {
					return false;
				}
				jArray.push_back(item);
				expectValue = false;
			}
			else if (*currItr != ' ' && *currItr != '\n' && *currItr != '\t')
			{
				return false;
			}
		};
		}
	}
	return false;
}
