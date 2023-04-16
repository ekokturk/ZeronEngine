// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron
{
	vk::SampleCountFlagBits VulkanHelpers::GetMultiSamplingLevel(MSAALevel level)
	{
		switch (level) {
			case MSAALevel::Disabled: return vk::SampleCountFlagBits::e1;
			case MSAALevel::x2: return vk::SampleCountFlagBits::e2;
			case MSAALevel::x4: return vk::SampleCountFlagBits::e4;
			case MSAALevel::x8: return vk::SampleCountFlagBits::e8;
			default: ZE_FAIL("Vulkan multi-sampling level is not supported!");
		}
		return vk::SampleCountFlagBits();
	}

	vk::PrimitiveTopology VulkanHelpers::GetPrimitiveTopology(PrimitiveTopology topology)
	{
		switch (topology) {
			case PrimitiveTopology::PointList: return vk::PrimitiveTopology::ePointList;
			case PrimitiveTopology::TriangleList: return vk::PrimitiveTopology::eTriangleList;
			case PrimitiveTopology::Invalid:;
			default: ZE_FAIL("Vulkan primitive topology is not supported");
		}
		return vk::PrimitiveTopology::eTriangleList;
	}

	vk::CullModeFlagBits VulkanHelpers::GetCullMode(FaceCullMode cullMode)
	{
		switch (cullMode) {
			case FaceCullMode::Back: return vk::CullModeFlagBits::eBack;
			case FaceCullMode::Front: return vk::CullModeFlagBits::eFront;
			case FaceCullMode::FrontAndBack: return vk::CullModeFlagBits::eFrontAndBack;
			case FaceCullMode::None:
			default: return vk::CullModeFlagBits::eNone;
		}
	}

	vk::BufferUsageFlagBits VulkanHelpers::GetBufferType(BufferType bufferType)
	{
		switch (bufferType) {
			case BufferType::Undefined: return static_cast<vk::BufferUsageFlagBits>(0);
			case BufferType::Vertex: return vk::BufferUsageFlagBits::eVertexBuffer;
			case BufferType::Index: return vk::BufferUsageFlagBits::eIndexBuffer;
			case BufferType::Uniform: return vk::BufferUsageFlagBits::eUniformBuffer;
			default: ZE_FAIL("Vulkan buffer type is not supported!");
		}
		return static_cast<vk::BufferUsageFlagBits>(0);
	}

	vk::ShaderStageFlagBits VulkanHelpers::GetShaderStage(ShaderType shaderType)
	{
		switch (shaderType) {
			case ShaderType::Vertex: return vk::ShaderStageFlagBits::eVertex;
			case ShaderType::Fragment: return vk::ShaderStageFlagBits::eFragment;
			case ShaderType::Compute: return vk::ShaderStageFlagBits::eCompute;
			default: ZE_FAIL("Vulkan shader type is not supported!");
		}
		return vk::ShaderStageFlagBits::eAll;
	}

	vk::DescriptorType VulkanHelpers::GetDescriptorType(PipelineResourceType resourceType)
	{
		switch (resourceType) {
			case PipelineResourceType::UniformBuffer: return vk::DescriptorType::eUniformBuffer;
			case PipelineResourceType::DynamicUniformBuffer: return vk::DescriptorType::eUniformBufferDynamic;
			case PipelineResourceType::Sampler: return vk::DescriptorType::eSampler;
			case PipelineResourceType::Texture: return vk::DescriptorType::eSampledImage;
			default: ZE_FAIL("Vulkan descriptor type is not supported!");
		}
		return vk::DescriptorType::eUniformBuffer;
	}

	vk::SamplerAddressMode VulkanHelpers::GetSamplerAddressMode(SamplerAddressMode addressMode)
	{
		switch (addressMode) {
			case SamplerAddressMode::Repeat: return vk::SamplerAddressMode::eRepeat;
			case SamplerAddressMode::Clamp: return vk::SamplerAddressMode::eClampToEdge;
			default: ZE_FAIL("Vulkan sampler address mode is not supported!");
		}
		return vk::SamplerAddressMode::eRepeat;
	}
}
#endif