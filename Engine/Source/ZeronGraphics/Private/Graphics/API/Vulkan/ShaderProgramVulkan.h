// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/ShaderProgram.h>
#	include <Graphics/VertexLayout.h>

namespace Zeron::Gfx
{
	class ShaderVulkan;
	class VertexLayout;
	class GraphicsVulkan;

	class ShaderProgramVulkan final : public ShaderProgram {
	  public:
		ShaderProgramVulkan(GraphicsVulkan& graphics, const ShaderProgramConfig& config, std::unordered_map<ShaderType, ByteBuffer> shaderData);
		~ShaderProgramVulkan() = default;

		// ShaderProgram
		Shader* GetShader(ShaderType type) const override;

		// Vulkan API
		std::vector<vk::PipelineShaderStageCreateInfo> GetPipelineStageInfoVK() const;
		vk::PipelineVertexInputStateCreateInfo GetVertexInputDescriptionVK() const;

	  private:
		void _createVertexInputDesc();
		vk::Format _getVertexAttributeFormat(VertexFormat format) const;
		ByteBuffer _readShaderFile(const std::filesystem::path& path) const;

		std::shared_ptr<ShaderVulkan> mVertexShader;
		std::shared_ptr<ShaderVulkan> mFragmentShader;
		std::shared_ptr<ShaderVulkan> mComputeShader;
		std::vector<vk::VertexInputAttributeDescription> mAttributeDescList;
		std::vector<vk::VertexInputBindingDescription> mBindingDescList;
	};
}
#endif