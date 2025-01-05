// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/GraphicsTypes.h>
#	include <Graphics/PipelineBinding.h>

namespace Zeron::Gfx
{
	class PipelineVulkan;
	class Pipeline;
	class PipelineLayoutVulkan;
	class Texture;
	class Buffer;
	class GraphicsVulkan;

	class PipelineBindingVulkan final : public PipelineBinding {
	  public:
		PipelineBindingVulkan(GraphicsVulkan& graphics, PipelineVulkan& pipeline, const std::vector<BindingHandle>& bindings);

		uint32_t GetBindingSetCount() const;

		// Vulkan API
		const std::vector<vk::UniqueDescriptorSet>& GetDescriptorSets() const;

	  private:
		void _countBindings(const std::vector<BindingHandle>& bindings, uint32_t& uniformBuffer, uint32_t& sampler, uint32_t& texture, uint32_t& structure) const;

		vk::UniqueDescriptorPool mDescriptorPool;
		std::vector<vk::UniqueDescriptorSet> mDescriptorSets;
	};
}
#endif