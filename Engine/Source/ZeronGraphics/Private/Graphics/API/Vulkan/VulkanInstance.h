// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>

namespace Zeron::Gfx
{
	class VulkanInstance {
	  public:
		static void LoadProc();
		static vk::Instance CreateRef(vk::InstanceCreateInfo createInfo);
		static void DestroyRef();

	  private:
		static vk::Instance mInstance;
		static uint32_t mRefCount;

#	if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		// vk::DynamicLoader alternative to search for various vulkan libraries
		class DynamicLoader {
		  public:
			DynamicLoader();
			~DynamicLoader();

			PFN_vkGetInstanceProcAddr GetInstanceProcAddress() const;
			bool IsSuccess() const { return mSuccess; }

		  private:
			bool mSuccess;
			void* mLibrary;
		};
		static DynamicLoader mDynamicLoader;
#	endif

#	if ZE_DEBUG
		static vk::DebugUtilsMessengerEXT mDebugMessenger;
#	endif
	};
}
#endif