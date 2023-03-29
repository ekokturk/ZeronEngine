// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <fmt/chrono.h>
#include <fmt/core.h>

#define ZE_STRING(s) #s
#define ZE_XSTRING(s) ZE_STRING(s)

namespace Zeron::Util {

	template<typename ...Args>
	std::string Format(std::string_view message, Args&&... args)
	{
		return fmt::vformat(message, fmt::make_format_args(args...));
	}

}
