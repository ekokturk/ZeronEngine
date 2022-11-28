// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/DispatchLoaderVulkan.h>

namespace Zeron
{
#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
	vk::DynamicLoader DispatchLoaderVulkan::dl;
#endif

	void DispatchLoaderVulkan::InitInstanceDispatcher()
	{
	#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
	#endif
	}

	void DispatchLoaderVulkan::InitInstanceDispatcher(vk::Instance& instance)
	{
	#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
			VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
	#endif
	}

}
#endif
