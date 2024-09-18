// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/GraphicsTypes.h>
#	include <Graphics/Sampler.h>

namespace Zeron::Gfx
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