// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanDebug.h>
#	include <Graphics/API/Vulkan/VulkanInstance.h>

namespace Zeron::Gfx
{
	vk::Instance VulkanInstance::mInstance;
	uint32_t VulkanInstance::mRefCount = 0;

#	if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
	vk::DynamicLoader VulkanInstance::mDynamicLoader;
#	endif

#	if ZE_DEBUG
	vk::DebugUtilsMessengerEXT VulkanInstance::mDebugMessenger;
#	endif

	void VulkanInstance::LoadProc()
	{
#	if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		ZE_ASSERT(mDynamicLoader.success(), "Unable to find Vulkan library on this platform");
		auto vkGetInstanceProcAddr = mDynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#	endif
	}

	vk::Instance VulkanInstance::CreateRef(vk::InstanceCreateInfo createInfo)
	{
		if (mRefCount == 0) {
			ZE_ASSERT(!mInstance, "Vulkan instance shouldn't exist with not references");
			mInstance = vk::createInstance(createInfo);

#	if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
			VULKAN_HPP_DEFAULT_DISPATCHER.init(mInstance);
#	endif

#	if ZE_DEBUG
			mDebugMessenger = CreateVulkanDebugUtilsMessenger(mInstance);
#	endif
		}
		++mRefCount;
		return mInstance;
	}

	void VulkanInstance::DestroyRef()
	{
		if (mRefCount != 0) {
			--mRefCount;
		}
		if (mRefCount == 0 && mInstance) {
#	if ZE_DEBUG
			mInstance.destroyDebugUtilsMessengerEXT(mDebugMessenger);
			mDebugMessenger = nullptr;
#	endif
			mInstance.destroy();
			mInstance = nullptr;
		}
	}
}
#endif