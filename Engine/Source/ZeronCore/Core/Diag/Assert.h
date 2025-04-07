// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/Diag/Logger.h>
#include <Core/Util.h>

#ifdef _MSVC_LANG
#	define ZE_DEBUG_BREAK() __debugbreak()
#else
#	define ZE_DEBUG_BREAK() ((void)0)
#endif

#if ZE_DEBUG
#	define ZE_LINE_INFO() "\n  FILE: " ZE_XSTRING(__FILE__) "\n  LINE: " ZE_XSTRING(__LINE__) "\n"
#	define ZE_DEBUG_ERROR(error, ...) \
		ZE_LOGE(error ZE_LINE_INFO()); \
		ZE_DEBUG_BREAK();
#else
#	define ZE_LINE_INFO() ""
#	define ZE_DEBUG_ERROR(error, ...)
#endif

#define ZE_EXPECT(condition, ...)                \
	do {                                         \
		if (!(condition)) {                      \
			ZE_LOGE(__VA_ARGS__);                \
			ZE_DEBUG_ERROR("UNEXPECTED VALUE!"); \
		}                                        \
	}                                            \
	while (0)

#define ZE_ASSERT(condition, ...)                \
	do {                                         \
		if (!(condition)) {                      \
			ZE_LOGE(__VA_ARGS__);                \
			ZE_DEBUG_ERROR("ASSERTION FAILED!"); \
			std::abort();                        \
		}                                        \
	}                                            \
	while (0)

#define ZE_FAIL(...)            \
	ZE_LOGE(__VA_ARGS__);       \
	ZE_DEBUG_ERROR("FAILURE!"); \
	std::abort()