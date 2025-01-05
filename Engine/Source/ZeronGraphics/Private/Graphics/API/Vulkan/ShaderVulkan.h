// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/Shader.h>
#	include <Graphics/VertexLayout.h>

namespace Zeron::Gfx
{
	class VertexLayout;
	class GraphicsVulkan;

	class ShaderVulkan final : public Shader {
	  public:
		ShaderVulkan(GraphicsVulkan& graphics, ShaderType type, const ByteBuffer& buffer);
		~ShaderVulkan() = default;

		// Vulkan API
		const vk::PipelineShaderStageCreateInfo& GetPipelineStageInfoVK() const;

	  protected:
		ByteBuffer mBuffer;
		vk::UniqueShaderModule mShader;
		vk::PipelineShaderStageCreateInfo mPipelineStageInfo;
	};
}
#endif