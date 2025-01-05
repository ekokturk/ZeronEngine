// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/PipelineVulkan.h>

#	include <Graphics/PipelineConfig.h>
#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/RenderPassVulkan.h>
#	include <Graphics/API/Vulkan/ShaderProgramVulkan.h>
#	include <Graphics/API/Vulkan/ShaderVulkan.h>
#	include <Graphics/API/Vulkan/VulkanHelpers.h>
#	include <Graphics/PushConstant.h>

namespace Zeron::Gfx
{
	PipelineVulkan::PipelineVulkan(GraphicsVulkan& graphics, ShaderProgramVulkan* shader, RenderPassVulkan* renderPass, PipelineConfig&& pipelineConfig)
		: Pipeline(std::move(pipelineConfig))
		, mShader(shader)
	{
		ZE_ASSERT(mShader, "Vulkan pipeline requires a shader program!");

		const vk::Device& device = graphics.GetDeviceVK();
		const PipelineConfig& config = *GetConfig();

		const std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = mShader->GetPipelineStageInfoVK();
		const vk::PipelineVertexInputStateCreateInfo vertexInput = mShader->GetVertexInputDescriptionVK();

		_createPipelineLayout(device);

		const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState(
			vk::PipelineInputAssemblyStateCreateFlags(), VulkanHelpers::GetPrimitiveTopology(config.mTopology), 0
		);

		std::vector<vk::DynamicState> dynamicStates = {
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor,
		};

		// Create the dynamic state create info structure
		vk::PipelineDynamicStateCreateInfo pipelineDynamicState({}, static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data());
		const vk::PipelineViewportStateCreateInfo pipelineViewportState(vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr);

		const vk::PipelineRasterizationStateCreateInfo rasterizationState(
			vk::PipelineRasterizationStateCreateFlags(),
			VK_FALSE,
			VK_FALSE,
			config.mSolidFill ? vk::PolygonMode::eFill : vk::PolygonMode::eLine,
			VulkanHelpers::GetCullMode(config.mCullMode),
			vk::FrontFace::eClockwise,
			VK_FALSE,
			0.0f,
			0.0f,
			0.0f,
			1.0f
		);

		const vk::PipelineMultisampleStateCreateInfo multiSampleState(
			vk::PipelineMultisampleStateCreateFlags(),
			VulkanHelpers::GetMultiSamplingLevel(config.mSamplingLevel),
			config.mSamplingLevel != MSAALevel::Disabled ? VK_TRUE : VK_FALSE,
			config.mSamplingLevel != MSAALevel::Disabled ? 0.2f : 0.0f,
			nullptr,
			VK_FALSE,
			VK_FALSE
		);

		const vk::PipelineDepthStencilStateCreateInfo depthStencilState(
			vk::PipelineDepthStencilStateCreateFlags(),
			config.mDepthMode == DepthMode::Disabled ? VK_FALSE : VK_TRUE,
			config.mDepthMode == DepthMode::Default ? VK_TRUE : VK_FALSE,
			vk::CompareOp::eLessOrEqual,
			VK_FALSE,
			VK_FALSE,
			vk::StencilOpState(),
			vk::StencilOpState(),
			0.0f,
			0.0f
		);

		const vk::ColorComponentFlags colorWriteMask{ vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
													  vk::ColorComponentFlagBits::eA };

		const vk::PipelineColorBlendAttachmentState colorBlendAttachment(
			VK_TRUE,
			vk::BlendFactor::eSrcAlpha,
			vk::BlendFactor::eOneMinusSrcAlpha,
			vk::BlendOp::eAdd,
			vk::BlendFactor::eOne,
			vk::BlendFactor::eZero,
			vk::BlendOp::eAdd,
			colorWriteMask
		);

		vk::PipelineColorBlendStateCreateInfo colorBlendState(
			vk::PipelineColorBlendStateCreateFlags(), VK_FALSE, vk::LogicOp::eClear, 1, &colorBlendAttachment, { { 0, 0, 0, 0 } }
		);

		const vk::GraphicsPipelineCreateInfo pipelineInfo(
			vk::PipelineCreateFlags(),
			static_cast<uint32_t>(shaderStages.size()),
			shaderStages.data(),
			&vertexInput,
			&inputAssemblyState,
			nullptr,
			&pipelineViewportState,
			&rasterizationState,
			&multiSampleState,
			&depthStencilState,
			&colorBlendState,
			&pipelineDynamicState,
			*mPipelineLayout,
			renderPass->GetRenderPassVK()
		);
		auto result = graphics.GetDeviceVK().createGraphicsPipelineUnique(nullptr, pipelineInfo);
		ZE_ASSERT(result, "PipelineVulkan: Graphics pipeline creation failed!");
		mPipeline = std::move(result);
	}

	PipelineVulkan::PipelineVulkan(GraphicsVulkan& graphics, ShaderProgramVulkan* shader)
		: Pipeline()
		, mShader(shader)
	{
		const vk::Device& device = graphics.GetDeviceVK();
		const std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = mShader->GetPipelineStageInfoVK();
		ZE_ASSERT(shaderStages.size() == 1 && mShader->GetShader(ShaderType::Compute), "Vulkan compute pipeline should be initialized with only compute shader");
		_createPipelineLayout(device);

		const vk::ComputePipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(), shaderStages.front(), *mPipelineLayout);

		auto result = graphics.GetDeviceVK().createComputePipelineUnique(nullptr, pipelineInfo);
		ZE_ASSERT(result, "PipelineVulkan: Compute pipeline creation failed!");
		mPipeline = std::move(result);
	}

	PipelineVulkan::~PipelineVulkan() {}

	const std::vector<ResourceLayout::Element>& PipelineVulkan::GetResourceLayout() const
	{
		ZE_ASSERT(mShader, "Vulkan pipeline needs to have a shader program!");
		return mShader->GetResourceLayout();
	}

	const PushConstant::Element* PipelineVulkan::GetPushConstant(ShaderType shaderType) const
	{
		return mShader->GetPushConstant(shaderType);
	}

	vk::Pipeline& PipelineVulkan::GetPipelineVK()
	{
		return *mPipeline;
	}

	vk::PipelineLayout& PipelineVulkan::GetPipelineLayoutVK()
	{
		return *mPipelineLayout;
	}

	const std::vector<vk::UniqueDescriptorSetLayout>& PipelineVulkan::GetDescriptorSetLayouts() const
	{
		return mDescriptorSetLayouts;
	}

	void PipelineVulkan::_createPipelineLayout(const vk::Device& device)
	{
		// Create binding list for each set
		std::vector<std::vector<vk::DescriptorSetLayoutBinding>> setLayouts;
		for (const auto& resource : mShader->GetResourceLayout()) {
			ZE_ASSERT(resource.mSet < 4, "More than 4 Descriptor sets is not supported!");
			if (setLayouts.size() < resource.mSet + 1) {
				setLayouts.emplace_back(std::vector<vk::DescriptorSetLayoutBinding>{});
			}
			auto& bindings = setLayouts[resource.mSet];
			bindings.emplace_back(resource.mBinding, VulkanHelpers::GetDescriptorType(resource.mType), 1, VulkanHelpers::GetShaderStage(resource.mShaderStage));
		}

		for (const auto& setLayout : setLayouts) {
			mDescriptorSetLayouts.emplace_back(
				device.createDescriptorSetLayoutUnique(vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), setLayout.size(), setLayout.data()))
			);
		}

		uint32_t pushConstantOffset = 0;
		std::vector<vk::PushConstantRange> pushConstantRanges;
		for (const auto& pushConstant : mShader->GetPushConstants()) {
			pushConstantRanges.emplace_back(VulkanHelpers::GetShaderStage(pushConstant.mShaderStage), pushConstantOffset, pushConstant.mStride);
			pushConstantOffset += pushConstant.mStride;
		}

		const auto rawLayouts = vk::uniqueToRaw(mDescriptorSetLayouts);
		const vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
			vk::PipelineLayoutCreateFlags(),
			static_cast<uint32_t>(mDescriptorSetLayouts.size()),
			rawLayouts.data(),
			static_cast<uint32_t>(pushConstantRanges.size()),
			pushConstantRanges.empty() ? nullptr : pushConstantRanges.data()
		);
		mPipelineLayout = device.createPipelineLayoutUnique(pipelineLayoutInfo);
	}
}
#endif