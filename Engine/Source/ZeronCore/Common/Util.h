// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <fmt/core.h>
#include <fmt/chrono.h>

namespace Zeron::Util {

	template<typename ...Args>
	std::string Format(std::string_view message, Args&&... args)
	{
		return fmt::format(message, args...);
	}

}
