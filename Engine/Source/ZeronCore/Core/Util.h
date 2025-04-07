// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/Types/TypeTraits.h>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <regex>

#define ZE_STRING(s)  #s
#define ZE_XSTRING(s) ZE_STRING(s)

namespace Zeron::Util
{
	template <typename... Args>
	std::string Format(std::string_view message, Args&&... args)
	{
		return fmt::vformat(message, fmt::make_format_args(args...));
	}

	inline bool IsNumber(const std::string& str)
	{
		static const std::regex numPattern("-?\\d+(\\.\\d+)?");
		return std::regex_match(str, numPattern);
	}

	inline bool IsBool(const std::string& str)
	{
		static const std::regex boolPattern("(true|false)");
		return std::regex_match(str, boolPattern);
	}

	template <typename T>
	constexpr std::string_view ToString(T val)
	{
		static_assert(TypeTraits::false_v<T>, "ToString not defined for this type");
		return "";
	}
}

#define ZE_ENUM_STRING_FORMAT(EnumType)                                                       \
	template <>                                                                               \
	struct fmt::formatter<EnumType> : fmt::formatter<fmt::string_view> {                      \
		template <typename FormatContext>                                                     \
		auto format(EnumType c, FormatContext& ctx)                                           \
		{                                                                                     \
			return fmt::formatter<fmt::string_view>::format(::Zeron::Util::ToString(c), ctx); \
		}                                                                                     \
	}