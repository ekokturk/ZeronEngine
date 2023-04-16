// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/SamplerVulkan.h>

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron
{
	SamplerVulkan::SamplerVulkan(GraphicsVulkan& graphics, SamplerAddressMode addressMode, bool hasAnisotropicFilter)
	{
		// TODO: Check graphics capabilities for hasAnisotropicFilter
		// TODO: Parameterize max anisotropy

		const vk::SamplerCreateInfo info(
			vk::SamplerCreateFlags(),
			vk::Filter::eLinear,
			vk::Filter::eLinear,
			vk::SamplerMipmapMode::eLinear,
			VulkanHelpers::GetSamplerAddressMode(addressMode),
			VulkanHelpers::GetSamplerAddressMode(addressMode),
			VulkanHelpers::GetSamplerAddressMode(addressMode),
			0.0f,
			hasAnisotropicFilter,
			8.0f,
			VK_FALSE,
			vk::CompareOp::eNever,
			0.0f,
			0.0f,
			// static_cast<float>(GetMipLevelVK()),
			vk::BorderColor::eIntOpaqueBlack,
			VK_FALSE
		);

		mSampler = graphics.GetDeviceVK().createSamplerUnique(info);
	}

	vk::Sampler& SamplerVulkan::GetSamplerVK()
	{
		return *mSampler;
	}
}
#endif