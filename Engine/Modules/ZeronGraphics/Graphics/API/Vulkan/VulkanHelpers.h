// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanCommon.h>
#include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	class VulkanHelpers {
	public:
		// Zeron -> Vulkan
		static vk::SampleCountFlagBits GetMultiSamplingLevel(MSAALevel level);
		static vk::PrimitiveTopology GetPrimitiveTopology(PrimitiveTopology topology);
		static vk::CullModeFlagBits GetCullMode(FaceCullMode cullMode);
		static vk::BufferUsageFlagBits GetBufferType(BufferType bufferType);
		static vk::ShaderStageFlagBits GetShaderStage(ShaderType shaderType);
		static vk::DescriptorType GetDescriptorType(PipelineResourceType resourceType);
		static vk::SamplerAddressMode GetSamplerAddressMode(SamplerAddressMode addressMode);
	};
}
#endif
