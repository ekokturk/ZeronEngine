// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptLibrary.h>

namespace Zeron::Script
{
	enum class RuntimeType;
	class Runtime;

	class ScriptEngine {
	  public:
		ScriptEngine();
		~ScriptEngine();

		std::unique_ptr<Runtime> CreateRuntime(RuntimeType type);

	  private:
		std::unordered_map<RuntimeType, std::unique_ptr<Library>> mLibraries;
	};
}