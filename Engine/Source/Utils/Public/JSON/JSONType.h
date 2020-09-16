// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

struct JSONType;

/*
 * TODO: Make Singleton
 */
struct JSONNull {};

/*
 * Wrapper class for boolean type (For variant type collusion)
 */
struct JSONBool
{
	JSONBool& operator=(bool v) { this->Value = v; return *this; }
	operator bool() const { return Value; }
	bool Value;
};


using JSONObject = std::map<std::string, JSONType>;
using JSONArray = std::vector<JSONType >;


struct JSONType
{
	using JSONTypeValue = std::variant<JSONNull, JSONObject, JSONArray, std::string, int, double, JSONBool>;

	JSONType() = default;
	JSONType(const JSONType& other) = default;
	JSONType(JSONType&&) = default;
	~JSONType() = default;
	JSONType& operator=(const JSONType& otherType) = default;
	JSONType& operator=(JSONTypeValue&& otherValue) { Value = otherValue; return *this; }

	/*
	 * Get value of the JSON type,
	 * if value is not correct throw an exception
	 */
	template<typename T>
	T Get() const
	{
		return std::get<T>(Value);
	}

	/*
	 * Get value of the JSON type with a check,
	 * Return value will be true only if the type checks out
	 */
	template<typename T>
	bool Get(T& inValue)
	{
		if (auto value = std::get_if<T>(&Value))
		{
			inValue = *value;
		}

		return false;
	}

	/*
	 * If JSON type is an object get the type by key value
	 */
	JSONType& operator[](const std::string& key)
	{
		if (JSONObject* object = std::get_if<JSONObject>(&Value))
		{
			const auto itr = (*object).find(key);
			if (itr != (*object).end())
			{
				return itr->second;
			}
		}

		return Null;
	}

	/*
	 * If JSON type is an object get the type by index value
	 */
	JSONType& operator[](unsigned int index)
	{
		if (JSONArray* object = std::get_if<JSONArray>(&Value))
		{
			if (index < (*object).size())
			{
				return (*object)[index];
			}
		}

		return Null;
	}

	/*
	 * Check if the value exists (non null)
	 */
	bool IsValid() const
	{
		return Value.index() != 0;
	}

	operator bool() const { return IsValid(); }

	std::string ToString() const;

	static JSONType NullType() { return Null; };

	JSONTypeValue  Value;

private:
	static JSONType Null;
	
};
