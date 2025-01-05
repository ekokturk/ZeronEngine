// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/Pipeline.h>
#	include <Graphics/PushConstant.h>
#	include <Graphics/ResourceLayout.h>

namespace Zeron::Gfx
{
	struct PipelineConfig;
	class PipelineLayoutVulkan;
	class PipelineLayout;
	class RenderPassVulkan;
	class ShaderProgramVulkan;
	class GraphicsVulkan;

	class PipelineVulkan final : public Pipeline {
	  public:
		PipelineVulkan(GraphicsVulkan& graphics, ShaderProgramVulkan* shader, RenderPassVulkan* renderPass, PipelineConfig&& pipelineConfig);
		PipelineVulkan(GraphicsVulkan& graphics, ShaderProgramVulkan* shader);
		~PipelineVulkan();

		const std::vector<ResourceLayout::Element>& GetResourceLayout() const;
		const PushConstant::Element* GetPushConstant(ShaderType shaderType) const;

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
	};
}
#endif