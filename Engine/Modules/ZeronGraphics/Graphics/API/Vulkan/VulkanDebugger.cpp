// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanDebugger.h>

namespace Zeron
{
#if ZE_DEBUG
	vk::DebugUtilsMessengerEXT VulkanDebugger::mDebugMessenger = nullptr;

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData)
	{
		ZE_LOG("Vulkan {}: {}", vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(severity)).c_str(), callbackData->pMessage);
		return false;
	}

	void VulkanDebugger::Create(const vk::Instance& instance)
	{
		ZE_ASSERT(!mDebugMessenger, "Vulkan debugger is already initialized!");
		const vk::DebugUtilsMessengerCreateInfoEXT messengerCreateInfo(
			vk::DebugUtilsMessengerCreateFlagsEXT{},
			/*vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |*/
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			DebugMessageCallback, nullptr);
		mDebugMessenger = instance.createDebugUtilsMessengerEXT(messengerCreateInfo);
	}

	void VulkanDebugger::Destroy(const vk::Instance& instance)
	{
		instance.destroyDebugUtilsMessengerEXT(mDebugMessenger);
		mDebugMessenger = nullptr;
	}
#endif
}
#endif
