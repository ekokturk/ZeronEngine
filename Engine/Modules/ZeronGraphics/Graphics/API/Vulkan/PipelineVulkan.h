// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanCommon.h>

#include <Graphics/GraphicsTypes.h>
#include <Graphics/Pipeline.h>
#include <Graphics/ResourceLayout.h>

namespace Zeron
{
	class PipelineLayoutVulkan;
	class PipelineLayout;
	class RenderPassVulkan;
	class ShaderProgramVulkan;
	class GraphicsVulkan;

	class PipelineVulkan final : public Pipeline {
	public:
		PipelineVulkan(
			GraphicsVulkan& graphics,
			ShaderProgramVulkan* shader, 
			RenderPassVulkan* renderPass,
			MSAALevel samplingLevel,
			PrimitiveTopology topology = PrimitiveTopology::TriangleList,
			bool isSolidFill = true,
			FaceCullMode cullMode = FaceCullMode::Back
		);
		PipelineVulkan(GraphicsVulkan& graphics, ShaderProgramVulkan* shader);
		~PipelineVulkan();

		const ResourceLayout& GetResourceLayout() const;

		// Vulkan API
		vk::Pipeline& GetPipelineVK();
		vk::PipelineLayout& GetPipelineLayoutVK();
		const std::vector<vk::UniqueDescriptorSetLayout>& GetDescriptorSetLayouts() const;

	private:
		void _createPipelineLayout(const vk::Device& device);

		vk::UniquePipeline mPipeline;
		vk::UniquePipelineLayout mPipelineLayout;
		std::vector<vk::UniqueDescriptorSetLayout> mDescriptorSetLayouts;

		ShaderProgramVulkan* mShader;
		MSAALevel mMultiSamplingLevel;
		PrimitiveTopology mPrimitiveTopology;
		bool mIsSolidFill;
		FaceCullMode mCullMode;
	};
}
#endif
