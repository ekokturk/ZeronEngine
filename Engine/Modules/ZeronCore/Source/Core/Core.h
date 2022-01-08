// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include "Logger.h"

#define ZE_STRING(s) #s
#define ZE_XSTRING(s) ZE_STRING(s)

#ifdef _MSVC_LANG
#define ZE_DEBUG_BREAK() __debugbreak()
#else
#define ZE_DEBUG_BREAK() ((void)0)
#endif

#define ZE_LOGE_LINEINFO(message, ...) \
	ZE_LOGE(message "\n  FILE: " ZE_XSTRING(__FILE__) "\n  LINE: " ZE_XSTRING(__LINE__) "\n  " __VA_ARGS__)

#define ZE_EXPECT(condition, ...) do { if(!(condition)) { \
	ZE_LOGE_LINEINFO("UNEXPECTED VALUE!", __VA_ARGS__); \
	ZE_DEBUG_BREAK(); } } while(0)

#define ZE_ASSERT(condition, ...) do { if(!(condition)) { \
	ZE_LOGE_LINEINFO("ASSERTION FAILED!", __VA_ARGS__); \
	ZE_DEBUG_BREAK(); \
	std::abort(); } } while(0)

#define ZE_FAIL(...) \
	ZE_LOGE_LINEINFO("FAILURE!", __VA_ARGS__); \
	ZE_DEBUG_BREAK(); \
	std::abort()


