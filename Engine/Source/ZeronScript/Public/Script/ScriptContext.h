// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptValue.h>

namespace Zeron::Script
{
	class Context {
	  public:
		virtual ~Context() = default;

		virtual size_t GetId() = 0;

		virtual Value Run(std::string_view script) = 0;
	};
}