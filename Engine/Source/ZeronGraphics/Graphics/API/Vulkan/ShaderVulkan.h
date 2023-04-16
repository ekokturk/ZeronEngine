// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanCommon.h>

#include <Graphics/Shader.h>
#include <Graphics/VertexLayout.h>

namespace Zeron
{
	class VertexLayout;
	class GraphicsVulkan;

	class ShaderVulkan final : public Shader {
	public:
		ShaderVulkan(GraphicsVulkan& graphics, ShaderType type, const ByteBuffer& buffer);
		~ShaderVulkan() override = default;

		// Vulkan API
		const vk::PipelineShaderStageCreateInfo& GetPipelineStageInfoVK() const;

	protected:
		ByteBuffer mBuffer;
		vk::UniqueShaderModule mShader;
		vk::PipelineShaderStageCreateInfo mPipelineStageInfo;
	};

	class ShaderProgramVulkan final : public ShaderProgram {
	public:
		ShaderProgramVulkan(GraphicsVulkan& graphics, const std::string& shaderName, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout,
			const ByteBuffer& vertexShader = {}, const ByteBuffer& fragmentShader = {}, const ByteBuffer& computeShader = {});
		ShaderProgramVulkan(GraphicsVulkan& graphics, const std::string& shaderName, const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout);
		~ShaderProgramVulkan() override = default;

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
