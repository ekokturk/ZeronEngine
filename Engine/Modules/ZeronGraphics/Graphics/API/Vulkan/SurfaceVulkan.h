// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanCommon.h>

namespace Zeron
{
	class SurfaceVulkan {
	public:
		static vk::UniqueSurfaceKHR CreateSurfaceVK(const vk::Instance& instance, SystemHandle systemHandle);	
	};
}
#endif
