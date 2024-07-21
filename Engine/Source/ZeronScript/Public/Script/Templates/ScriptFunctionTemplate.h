// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptValue.h>

namespace Zeron::Script
{
	template <typename ReturnType, typename... Args, std::size_t... Indices>
	auto CallFreeFunction(ReturnType (*func)(Args...), const std::vector<Value>& values, std::index_sequence<Indices...>)
	{
		if constexpr (sizeof...(Args) > 0) {
			return func(values[Indices].template Get<Args>()...);
		}
		else {
			return func();
		}
	}

	template <typename ReturnType, typename Class, typename... Args, std::size_t... Indices>
	auto CallMemberFunction(ReturnType (Class::*func)(Args...), Class* caller, const std::vector<Value>& values, std::index_sequence<Indices...>)
	{
		if constexpr (sizeof...(Args) > 0) {
			return (caller->*func)(values[Indices].template Get<Args>()...);
		}
		else {
			return (static_cast<Class*>(caller)->*func)();
		}
	}

	class Runtime;
	template <typename ReturnType, typename... Args>
	auto CreateFunctionBinding(ReturnType (*func)(Args...))
	{
		return [func](const std::vector<Value>& values, void*) {
			if (values.size() != sizeof...(Args)) {
				throw std::invalid_argument("Error: Mismatched argument count!");
			}

			if constexpr (std::is_void_v<ReturnType>) {
				CallFreeFunction(func, values, std::index_sequence_for<Args...>{});
				return Value();
			}
			else {
				return Value(CallFreeFunction(func, values, std::index_sequence_for<Args...>{}));
			}
		};
	}

	template <typename Class, typename ReturnType, typename... Args>
	auto CreateFunctionBinding(ReturnType (Class::*func)(Args...))
	{
		return [func](const std::vector<Value>& values, void* caller) {
			if (values.size() != sizeof...(Args)) {
				throw std::invalid_argument("Error: Mismatched argument count!");
			}

			if constexpr (std::is_void_v<ReturnType>) {
				CallMemberFunction(func, static_cast<Class*>(caller), values, std::index_sequence_for<Args...>{});
				return Value();
			}
			else {
				return Value(CallMemberFunction(func, static_cast<Class*>(caller), values, std::index_sequence_for<Args...>{}));
			}
		};
	}

	struct FunctionTemplate {
		using Callback = std::function<Value(ValueList, void*)>;

		Callback mCallback;
	};
}