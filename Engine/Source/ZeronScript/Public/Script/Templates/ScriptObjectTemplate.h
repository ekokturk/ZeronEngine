// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptObjectManager.h>
#include <Script/Templates/ScriptFunctionTemplate.h>
#include <Script/Templates/ScriptPropertyTemplate.h>
#include <Script/TypeInfo.h>

namespace Zeron::Script
{
	class ObjectManager;

	template <typename Class, typename... Args>
	auto CreateConstructorBinding()
	{
		return [](ObjectManager& manager, ValueList&& values) {
			if (values.size() != sizeof...(Args)) {
				throw std::invalid_argument("Error: Mismatched argument count!");
			}

			if constexpr (sizeof...(Args) > 0) {
				ObjectInstance instance = manager.CreateInstance<Class>(values, std::index_sequence_for<Args...>{});
				return instance;
			}
			else {
				ObjectInstance instance = manager.CreateInstance<Class>();
				return instance;
			}
		};
	}

	struct ObjectConstructorTemplate {
		using Fn = std::function<ObjectInstance(ObjectManager&, ValueList&&)>;

		Fn mCallback;
	};

	struct ObjectTemplate {

		template <typename T>
		static ObjectTemplate Create()
		{
			return ObjectTemplate(TypeInfo::Create<T>());
		}

		ObjectTemplate(const TypeInfo& type)
			: mTypeInfo(type)
		{}

		template <typename Class, typename... Args>
		ObjectTemplate& AddConstructor()
		{
			mConstructor = { CreateConstructorBinding<Class, Args...>() };
			return *this;
		}

		template <typename Class, typename ReturnType, typename... Args>
		ObjectTemplate& AddFunction(std::string name, ReturnType (Class::*func)(Args...))
		{
			mFunctions.emplace(std::move(name), FunctionTemplate{ CreateFunctionBinding(func) });
			return *this;
		}

		template <typename ReturnType, typename... Args>
		ObjectTemplate& AddFunction(std::string name, ReturnType (*func)(Args...))
		{
			mFunctions.emplace(std::move(name), FunctionTemplate{ CreateFunctionBinding(func) });
			return *this;
		}

		template <typename Class, typename PropertyType>
		ObjectTemplate& AddProperty(std::string name, PropertyType Class::*property)
		{
			PropertyTemplate prop;
			prop.mGetter = CreatePropertyGetterBinding(property);
			prop.mSetter = CreatePropertySetterBinding(property);

			mProperties.emplace(std::move(name), prop);
			return *this;
		}

		template <typename Class, typename PropertyType>
		ObjectTemplate& AddPropertyReadOnly(std::string name, PropertyType Class::*property)
		{
			PropertyTemplate prop;
			prop.mGetter = CreatePropertyGetterBinding(property);
			mProperties.emplace(std::move(name), prop);
			return *this;
		}

		TypeInfo mTypeInfo;
		std::optional<ObjectConstructorTemplate> mConstructor;
		std::unordered_map<std::string, PropertyTemplate> mProperties;
		std::unordered_map<std::string, FunctionTemplate> mFunctions;
	};
}