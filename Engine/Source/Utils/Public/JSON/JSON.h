// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "JSONType.h"

/*
 * _________Example Initialization_________
   		JSON J{
			JSONObject{
				{"A",{JSONArray{
						{"Test"},
						{12.123e-2},
						JSONType::NullType()
				}}},
				{"B", {false}},
				{"C", {true}}
			}
		};
 */


struct JSON
{
	JSON() = default;
	JSON(const JSON& other) = default;
	JSON(JSON&&) = default;
	~JSON() = default;
	JSON& operator=(const JSON& otherType) = default;
	JSON& operator=(JSON&& otherType) = default;

	JSONType& operator[](const std::string& key) { return Data[key]; }
	JSONType& operator[](int index) { return Data[index]; }

	/*
	 * Parse a string in the form of JSON into a JSON Object
	 * If parsing fails, null object will be returned
	 */
	static JSON Parse(const std::string& content);

	/*
	 * TODO: Implement expanded string format
	 * Convent a JSON object to string format
	 * If isExpanded is true, spaces, tabs and end-lines will be added
	 */
	static std::string Stringify(const JSON& inJson, bool isExpanded);

	/*
	 * Export contents of JSON Object to file.
	 * If true, export is successful.
	 */
	static bool ExportToFile(const JSON& json, const std::string& filePath);

	/*
	 * TODO: Read Line by line
	 * Create JSON Object from file.
	 * If true, import is successful.
	 */
	static bool ImportFromFile(JSON& json, const std::string& filePath);

	// Return string representation of JSON object
	std::string ToString() const;
	
	JSONType Data;

private:

	/*// ================== STRING TO JSON HELPERS =====================
	 * Helper functions are used to iterate through the JSON string and convert string values into JSON Types
	 * Recursive parsing goes over each character and returns null object if parsing fails
	 */

	 // Internal method to parse JSON and check for invalid characters
	static bool Parse_Internal(JSONType& value, std::string::const_iterator& curr, std::string::const_iterator& end);

	// Parse a raw JSON value (Returns true if raw value is valid)
	static bool ParseValue(JSONType& value, std::string::const_iterator& curr, std::string::const_iterator& end);

	// Parse a number (int, float) as certain JSON type (Returns true if number is valid)
	static bool ParseNumber(JSONType& value, std::string::const_iterator& currItr, std::string::const_iterator& endItr);

	// Parse string as JSON Key or string (Returns true if string format is valid)
	static bool ParseString(std::string& str, std::string::const_iterator& curr, std::string::const_iterator& end);

	enum ExpectedObjectType
	{
		None, Key, Separator, Value
	};

	// Parse string as JSON Object (Returns true if object is valid)
	static bool ParseObject(JSONObject& jObject, std::string::const_iterator& curr, std::string::const_iterator& end);

	// Parse string as JSON Array (Returns true if array is valid)
	static bool ParseArray(JSONArray& jArray, std::string::const_iterator& curr, std::string::const_iterator& end);

	// Type to contain value and digit count of the parsed whole number
	struct WholeNumber
	{
		double Value = 0;
		int DigitCount = 0;
	};

	// Get the whole number from digit based character (until non digit character is read)
	static WholeNumber ParseWholeNumber(std::string::const_iterator& currItr, std::string::const_iterator& endItr);

};
