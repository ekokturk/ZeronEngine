// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanCommon.h>

namespace Zeron
{
	class DispatchLoaderVulkan {
	public:
		static void InitInstanceDispatcher();
		static void InitInstanceDispatcher(vk::Instance& instance);

	#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		static vk::DynamicLoader dl;
	#endif
	};

}
#endif
