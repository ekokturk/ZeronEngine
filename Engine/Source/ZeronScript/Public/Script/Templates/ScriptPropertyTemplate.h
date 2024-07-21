// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptValue.h>

namespace Zeron::Script
{
	template <typename PropertyType, typename Class>
	auto CreatePropertySetterBinding(PropertyType Class::*property)
	{
		return [property](const Value& value, void* caller) {
			static_cast<Class*>(caller)->*property = value.Get<PropertyType>();
		};
	}

	template <typename PropertyType, typename Class>
	auto CreatePropertyGetterBinding(PropertyType Class::*property)
	{
		return [property](void* caller) -> Value {
			return Value(static_cast<Class*>(caller)->*property);
		};
	}

	struct PropertyTemplate {
		using Getter = std::function<Value(void*)>;
		using Setter = std::function<void(Value, void*)>;

		Getter mGetter;
		Setter mSetter = nullptr;
	};
}