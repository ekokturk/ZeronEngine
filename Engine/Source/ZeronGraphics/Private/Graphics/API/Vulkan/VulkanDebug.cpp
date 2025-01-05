// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN && ZE_DEBUG

#	include <Graphics/API/Vulkan/VulkanDebug.h>

namespace Zeron::Gfx
{
	namespace VulkanDebug
	{
#	define VULKAN_DEBUG_MESSAGE_NEGATIVE_HEIGHT 2698765901U
		std::set<uint32_t> IgnoredMessages = { VULKAN_DEBUG_MESSAGE_NEGATIVE_HEIGHT };

		VKAPI_ATTR VkBool32 VKAPI_CALL MessageCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData
		)
		{
			if (IgnoredMessages.contains(callbackData->messageIdNumber)) {
				return false;
			}

			ZE_LOG("Vulkan {}: {}", vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(severity)).c_str(), callbackData->pMessage);
			return false;
		}
	}

	vk::DebugUtilsMessengerEXT CreateVulkanDebugUtilsMessenger(vk::Instance instance)
	{
		const vk::DebugUtilsMessengerCreateInfoEXT messengerCreateInfo(
			vk::DebugUtilsMessengerCreateFlagsEXT{},
			/*vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |*/
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			VulkanDebug::MessageCallback,
			nullptr
		);
		return instance.createDebugUtilsMessengerEXT(messengerCreateInfo);
	}

	VulkanObjectDebugInterface::VulkanObjectDebugInterface(vk::Device device)
		: mDevice(device)
	{}

	VulkanObjectDebugInterface::~VulkanObjectDebugInterface() = default;

	void VulkanObjectDebugInterface::SetDebugLabel(vk::ObjectType type, uint64_t handle, std::string_view label) const
	{
		const vk::DebugUtilsObjectNameInfoEXT nameInfo{ type, handle, label.data() };
		mDevice.setDebugUtilsObjectNameEXT(&nameInfo);
	}
}
#endif