// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/GraphicsAdapterVulkan.h>
#	include <Graphics/API/Vulkan/GraphicsVulkan.h>

namespace Zeron
{
	GraphicsAdapterVulkan::GraphicsAdapterVulkan(vk::PhysicalDevice physicalDevice)
		: mPhysicalDevice(physicalDevice)
		, mProperties(physicalDevice.getProperties())
	{}

	const char* GraphicsAdapterVulkan::GetName() const
	{
		return mProperties.deviceName;
	}

	bool GraphicsAdapterVulkan::HasSwapChainSupport() const
	{
		for (const auto& extension : mPhysicalDevice.enumerateDeviceExtensionProperties()) {
			if (std::strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
				return true;
			}
		}
		return false;
	}

	bool GraphicsAdapterVulkan::IsDiscreteAdapter() const
	{
		return mProperties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu;
	}

	vk::PhysicalDevice& GraphicsAdapterVulkan::GetPhysicalDeviceVK()
	{
		return mPhysicalDevice;
	}

	const vk::PhysicalDeviceProperties& GraphicsAdapterVulkan::GetPropertiesVK() const
	{
		return mProperties;
	}
}
#endif