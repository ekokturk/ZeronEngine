// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#	if ZE_PLATFORM_WIN32
#		define VK_USE_PLATFORM_WIN32_KHR
#	elif ZE_PLATFORM_LINUX
#		define VK_USE_PLATFORM_XLIB_KHR
#	elif ZE_PLATFORM_ANDROID
#		define VK_USE_PLATFORM_ANDROID_KHR
#	endif

#	pragma warning(push, 0)
#	include <vulkan/vulkan.hpp>
#	undef None
#	pragma warning(pop)

#	define ZE_VK_TIMEOUT UINT64_MAX

#	define ZE_VK_ASSERT(call, message, ...)      \
		do {                                      \
			const vk::Result result = (call);     \
			if (result != vk::Result::eSuccess) { \
				ZE_FAIL(message);                 \
				return __VA_ARGS__;               \
			}                                     \
		}                                         \
		while (0)

#endif