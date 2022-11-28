// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/PipelineVulkan.h>

#include <Graphics/API/Vulkan/GraphicsVulkan.h>
#include <Graphics/API/Vulkan/RenderPassVulkan.h>
#include <Graphics/API/Vulkan/ShaderVulkan.h>
#include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron
{
	PipelineVulkan::PipelineVulkan(GraphicsVulkan& graphics, ShaderProgramVulkan* shader, RenderPassVulkan* renderPass, MSAALevel samplingLevel, PrimitiveTopology topology, bool isSolidFill, FaceCullMode cullMode)
		: mShader(shader)
		, mMultiSamplingLevel(samplingLevel)
		, mPrimitiveTopology(topology)
		, mIsSolidFill(isSolidFill)
		, mCullMode(cullMode)
	{
		ZE_ASSERT(mShader, "Vulkan pipeline requires a shader program!");

		const vk::Device& device = graphics.GetDeviceVK();

        const std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = mShader->GetPipelineStageInfoVK();
        const vk::PipelineVertexInputStateCreateInfo vertexInput = mShader->GetVertexInputDescriptionVK();

		_createPipelineLayout(device);

		const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState(
		    vk::PipelineInputAssemblyStateCreateFlags(),
            VulkanHelpers::GetPrimitiveTopology(mPrimitiveTopology),
		    0
        );

		const std::array<vk::DynamicState, 2> dynamicStates = {
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};
		const vk::PipelineDynamicStateCreateInfo pipelineDynamicState(
			vk::PipelineDynamicStateCreateFlags(), dynamicStates
		);
		const vk::PipelineViewportStateCreateInfo pipelineViewportState(
			vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr
		);

        const vk::PipelineRasterizationStateCreateInfo rasterizationState(
		    vk::PipelineRasterizationStateCreateFlags(),
		    VK_FALSE,
		    VK_FALSE,
			mIsSolidFill ? vk::PolygonMode::eFill : vk::PolygonMode::eLine,
			VulkanHelpers::GetCullMode(mCullMode),
		    vk::FrontFace::eClockwise,
		    VK_FALSE,
		    0.0f, 
		    0.0f,
		    0.0f,
    1.0f 
		);

		const vk::PipelineMultisampleStateCreateInfo multiSampleState(
			vk::PipelineMultisampleStateCreateFlags(),
			VulkanHelpers::GetMultiSamplingLevel(mMultiSamplingLevel),
			mMultiSamplingLevel != MSAALevel::Disabled ? VK_TRUE : VK_FALSE,
			mMultiSamplingLevel != MSAALevel::Disabled ? 0.2f : 0.0f,
			nullptr,
			VK_FALSE,
			VK_FALSE
		);

		const vk::PipelineDepthStencilStateCreateInfo depthStencilState(
			vk::PipelineDepthStencilStateCreateFlags(),
			VK_TRUE,
			VK_TRUE,
			vk::CompareOp::eLessOrEqual,
			VK_FALSE,
			VK_FALSE,
			vk::StencilOpState(),
			vk::StencilOpState(),
			0.0f,
			0.0f
		);

		const vk::ColorComponentFlags colorWriteMask{
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA
		};

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
			vk::PipelineColorBlendStateCreateFlags(),
			VK_FALSE,
			vk::LogicOp::eClear,
			1,
			&colorBlendAttachment,
			{{0, 0, 0, 0}}
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
        vk::ResultValue result = graphics.GetDeviceVK().createGraphicsPipelineUnique(nullptr, pipelineInfo);
		ZE_ASSERT(result.result == vk::Result::eSuccess, "Vulkan graphics pipeline creation failed!");
		mPipeline = std::move(result.value);
	}

	PipelineVulkan::PipelineVulkan(GraphicsVulkan& graphics, ShaderProgramVulkan* shader)
		: mShader(shader)
		, mMultiSamplingLevel(MSAALevel::Disabled)
		, mPrimitiveTopology(PrimitiveTopology::Invalid)
		, mIsSolidFill(false)
		, mCullMode(FaceCullMode::None)
	{
		const vk::Device& device = graphics.GetDeviceVK();
		const std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = mShader->GetPipelineStageInfoVK();
		ZE_ASSERT(shaderStages.size() == 1 && mShader->GetShader(ShaderType::Compute), "Vulkan compute pipeline should be initialized with only compute shader");
		_createPipelineLayout(device);

		const vk::ComputePipelineCreateInfo pipelineInfo(
			vk::PipelineCreateFlags(),
			shaderStages.front(),
			*mPipelineLayout
		);

		vk::ResultValue result = graphics.GetDeviceVK().createComputePipelineUnique(nullptr, pipelineInfo);
		ZE_ASSERT(result.result == vk::Result::eSuccess, "Vulkan compute pipeline creation failed!");
		mPipeline = std::move(result.value);
	}

	PipelineVulkan::~PipelineVulkan()
	{
	}

	const ResourceLayout& PipelineVulkan::GetResourceLayout() const
	{
		ZE_ASSERT(mShader, "Vulkan pipeline needs to have a shader program!");
		return mShader->GetResourceLayout();
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
		for (const auto& resource : mShader->GetResourceLayout().GetResources()) {
			ZE_ASSERT(resource.mSet < 4, "More than 4 Descriptor sets is not supported!");
			if (setLayouts.size() < resource.mSet + 1) {
				setLayouts.emplace_back(std::vector<vk::DescriptorSetLayoutBinding>{});
			}
			auto& bindings = setLayouts[resource.mSet];
			bindings.emplace_back(vk::DescriptorSetLayoutBinding(
				resource.mBinding,
				VulkanHelpers::GetDescriptorType(resource.mType),
				1,
				VulkanHelpers::GetShaderStage(resource.mShaderStage))
			);
		}

		for (const auto& setLayout : setLayouts) {
			mDescriptorSetLayouts.emplace_back(
				device.createDescriptorSetLayoutUnique(
					vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), setLayout)
				)
			);
		}

		const auto rawLayouts = vk::uniqueToRaw(mDescriptorSetLayouts);
		const vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
			vk::PipelineLayoutCreateFlags(),
			static_cast<uint32_t>(mDescriptorSetLayouts.size()),
			rawLayouts.data(),
			0,
			nullptr
		);
		mPipelineLayout = device.createPipelineLayoutUnique(pipelineLayoutInfo);
	}
}
#endif
