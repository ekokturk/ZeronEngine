// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptObject.h>
#include <Script/ScriptObjectManager.h>
#include <Script/Templates/ScriptObjectTemplate.h>
#include <Script/TypeInfo.h>

namespace Zeron::Script
{
	struct ObjectFactoryTemplate {};

	struct GlobalTemplate {

		struct ObjectFactory {
			using Fn = std::function<ObjectInstance(ObjectManager&)>;

			TypeInfo mTypeInfo;
			Fn mCreateFn;
		};

		template <typename Class>
		GlobalTemplate& AddObjectInstance(std::string name, ObjectFactory::Fn callback)
		{
			mInstances.emplace(std::move(name), ObjectFactory{ TypeInfo::Create<Class>(), std::move(callback) });
			return *this;
		}

		GlobalTemplate& AddClass(std::string name, ObjectTemplate&& classTemplate)
		{
			mClasses.emplace(std::move(name), std::move(classTemplate));
			return *this;
		}

		template <typename ReturnType, typename... Args>
		GlobalTemplate& AddFunction(std::string name, ReturnType (*func)(Args...))
		{
			mFunctions.emplace(std::move(name), FunctionTemplate{ CreateFunctionBinding(func) });
			return *this;
		}

		std::unordered_map<std::string, ObjectFactory> mInstances;
		std::unordered_map<std::string, ObjectTemplate> mClasses;
		std::unordered_map<std::string, FunctionTemplate> mFunctions;
	};
}