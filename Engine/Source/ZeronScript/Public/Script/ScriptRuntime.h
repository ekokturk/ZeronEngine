// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/Templates/ScriptGlobalTemplate.h>

namespace Zeron::Script
{
	enum class RuntimeType;
	class Context;

	class Runtime {
	  public:
		virtual ~Runtime() = default;

		virtual Context* CreateContext(GlobalTemplate globalTemplate = {}) = 0;
		virtual bool RemoveContext(Context* context) = 0;

		virtual void EnterScope(Context* activeContext = nullptr) = 0;
		virtual void ExitScope() = 0;
		virtual bool IsInScope(Context* activeContext = nullptr) const = 0;

		virtual RuntimeType GetType() const = 0;

		virtual void ForceGarbageCollection() = 0;
	};
}