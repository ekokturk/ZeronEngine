// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Script/ScriptEngine.h>

#include <Script/JS/V8/ScriptLibraryV8.h>
#include <Script/ScriptRuntime.h>
#include <Script/ScriptRuntimeType.h>

namespace Zeron::Script
{

	ScriptEngine::ScriptEngine() {}

	ScriptEngine::~ScriptEngine()
	{
		mLibraries.clear();
	}

	std::unique_ptr<Runtime> ScriptEngine::CreateRuntime(RuntimeType type)
	{
		if (!mLibraries.contains(type)) {
			switch (type) {
				case RuntimeType::JavaScriptV8: {
					mLibraries.emplace(RuntimeType::JavaScriptV8, std::make_unique<LibraryV8>());
				} break;
				default: {
					ZE_FAIL("Script Library is not implemented");
					return nullptr;
				}
			}
		}

		const auto library = mLibraries.find(type);
		return library->second->CreateRuntime();
	}
}