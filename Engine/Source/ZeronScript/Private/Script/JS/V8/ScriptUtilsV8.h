// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptValue.h>
#include <v8-isolate.h>

namespace Zeron::Script
{
	struct UtilsV8 {
		static v8::Local<v8::String> FromString(v8::Isolate* isolate, std::string_view);

		static Value ToValue(v8::Isolate* isolate, v8::Local<v8::Value> val);
		static v8::Local<v8::Value> FromValue(v8::Isolate* isolate, Value& val);

		static v8::FunctionCallback CreateConstructorCallback();
		static v8::FunctionCallback CreateFunctionCallback();
		static v8::AccessorGetterCallback CreateGetterCallback();
		static v8::AccessorSetterCallback CreateSetterCallback();

		static void ThrowException(v8::Isolate* isolate, std::string_view message);
	};
}