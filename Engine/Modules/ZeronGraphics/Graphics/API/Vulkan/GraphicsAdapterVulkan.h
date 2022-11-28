// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/GraphicsAdapter.h>
#include <Graphics/API/Vulkan/VulkanCommon.h>

namespace Zeron
{
	class GraphicsVulkan;

	class GraphicsAdapterVulkan final : public GraphicsAdapter {
	public:
		GraphicsAdapterVulkan(vk::PhysicalDevice physicalDevice);

		const char* GetName() const override;
		bool HasSwapChainSupport() const override;
		bool IsDiscreteAdapter() const override;

		vk::PhysicalDevice& GetPhysicalDeviceVK();
		const vk::PhysicalDeviceProperties& GetPropertiesVK() const;

	private:
		vk::PhysicalDevice mPhysicalDevice;
		vk::PhysicalDeviceProperties mProperties;
	};
}
#endif
