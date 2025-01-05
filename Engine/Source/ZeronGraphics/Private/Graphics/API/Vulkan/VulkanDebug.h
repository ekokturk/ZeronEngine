// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN && ZE_DEBUG

#	include <Graphics/API/Vulkan/VulkanCommon.h>

namespace Zeron::Gfx
{
	vk::DebugUtilsMessengerEXT CreateVulkanDebugUtilsMessenger(vk::Instance instance);

	class VulkanObjectDebugInterface {
	  public:
		VulkanObjectDebugInterface(vk::Device device);
		~VulkanObjectDebugInterface();

		void SetDebugLabel(vk::ObjectType type, uint64_t handle, std::string_view label) const;

	  private:
		vk::Device mDevice;
	};

}
#endif