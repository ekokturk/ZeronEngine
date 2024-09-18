// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#	include <vulkan/vulkan.hpp>

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