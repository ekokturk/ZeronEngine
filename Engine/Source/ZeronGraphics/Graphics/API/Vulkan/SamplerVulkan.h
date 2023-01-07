// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanCommon.h>

#include <Graphics/Sampler.h>
#include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	class GraphicsVulkan;

	class SamplerVulkan final : public Sampler {
	public:
		SamplerVulkan(GraphicsVulkan& graphics, SamplerAddressMode addressMode, bool hasAnisotropicFilter);
		~SamplerVulkan() = default;

		// Vulkan API
		vk::Sampler& GetSamplerVK();

	private:
		vk::UniqueSampler mSampler;
	};
}
#endif
