// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron::Gfx
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
			case PrimitiveTopology::LineList: return vk::PrimitiveTopology::eLineList;
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
			case BufferType::Storage: return vk::BufferUsageFlagBits::eStorageBuffer;
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
			case PipelineResourceType::StorageBuffer: return vk::DescriptorType::eStorageBuffer;
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

	vk::Format VulkanHelpers::GetTextureFormat(TextureFormat textureFormat)
	{
		switch (textureFormat) {
			case TextureFormat::RGB_8U: return vk::Format::eR8G8B8Unorm;
			case TextureFormat::RGBA_8U: return vk::Format::eR8G8B8A8Unorm;
			case TextureFormat::BGRA_8U: return vk::Format::eB8G8R8A8Unorm;
			case TextureFormat::Depth_16U: return vk::Format::eD16Unorm;
			case TextureFormat::Depth_32F: return vk::Format::eD32Sfloat;
			case TextureFormat::DepthStencil_32U: return vk::Format::eD24UnormS8Uint;
			default: ZE_FAIL("Vulkan texture format is not supported!");
		}
		return vk::Format::eUndefined;
	}

	vk::ImageLayout VulkanHelpers::GetTextureLayout(TextureLayout textureLayout)
	{
		switch (textureLayout) {
			case TextureLayout::Present: return vk::ImageLayout::ePresentSrcKHR;
			case TextureLayout::ColorAttachment: return vk::ImageLayout::eColorAttachmentOptimal;
			case TextureLayout::DepthStencilAttachment: return vk::ImageLayout::eDepthStencilAttachmentOptimal;
			case TextureLayout::ShaderReadOnly: return vk::ImageLayout::eShaderReadOnlyOptimal;
			case TextureLayout::TransferDst: return vk::ImageLayout::eTransferDstOptimal;
			case TextureLayout::TransferSrc: return vk::ImageLayout::eTransferSrcOptimal;
			case TextureLayout::Undefined: return vk::ImageLayout::eUndefined;
			default: ZE_FAIL("Vulkan texture layout is not supported!");
		}
		return vk::ImageLayout::eUndefined;
	}
}
#endif