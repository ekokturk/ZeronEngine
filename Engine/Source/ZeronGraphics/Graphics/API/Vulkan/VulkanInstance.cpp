// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#include <Graphics/API/Vulkan/VulkanInstance.h>

namespace Zeron
{
	vk::Instance VulkanInstance::mInstance;
	uint32_t VulkanInstance::mRefCount = 0;

#if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
	vk::DynamicLoader VulkanInstance::mDynamicLoader;
#endif

#if ZE_DEBUG
	vk::DebugUtilsMessengerEXT VulkanInstance::mDebugMessenger;

	namespace VulkanDebug
	{
		#define VULKAN_DEBUG_MESSAGE_NEGATIVE_HEIGHT 2698765901U
		std::set<uint32_t> IgnoredMessages = {
			VULKAN_DEBUG_MESSAGE_NEGATIVE_HEIGHT
		};

		VKAPI_ATTR VkBool32 VKAPI_CALL MessageCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT type,
			const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
			void* userData)
		{
			if(IgnoredMessages.count(callbackData->messageIdNumber)) {
				return false;
			}

			ZE_LOG("Vulkan {}: {}", vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(severity)).c_str(), callbackData->pMessage);
			return false;
		}
	}
#endif


	void VulkanInstance::LoadProc()
	{
	#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
		ZE_ASSERT(mDynamicLoader.success(), "Unable to find Vulkan library on this platform");
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = mDynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
	#endif
	}

	vk::Instance VulkanInstance::CreateRef(vk::InstanceCreateInfo createInfo)
	{
		if (mRefCount == 0) {
			ZE_ASSERT(!mInstance, "Vulkan instance shouldn't exist with not references");
			mInstance = vk::createInstance(createInfo);

	#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
			VULKAN_HPP_DEFAULT_DISPATCHER.init(mInstance);
	#endif

	#if ZE_DEBUG
			const vk::DebugUtilsMessengerCreateInfoEXT messengerCreateInfo(
				vk::DebugUtilsMessengerCreateFlagsEXT{},
				/*vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |*/
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
				VulkanDebug::MessageCallback, nullptr);
			mDebugMessenger = mInstance.createDebugUtilsMessengerEXT(messengerCreateInfo);
	#endif
		}
		++mRefCount;
		return mInstance;
	}

	void VulkanInstance::DestroyRef()
	{
		if (mRefCount != 0) {
			--mRefCount;
		}
		if(mRefCount == 0 && mInstance) {
	#if ZE_DEBUG
			mInstance.destroyDebugUtilsMessengerEXT(mDebugMessenger);
			mDebugMessenger = nullptr;
	#endif
			mInstance.destroy();
			mInstance = nullptr;
		}
	}
}
#endif
