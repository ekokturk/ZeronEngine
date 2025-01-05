// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/ShaderProgramVulkan.h>

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/VulkanHelpers.h>
#	include <Graphics/VertexLayout.h>
#	include <Graphics/API/Vulkan/ShaderVulkan.h>

namespace Zeron::Gfx
{
	ShaderProgramVulkan::ShaderProgramVulkan(GraphicsVulkan& graphics, const ShaderProgramConfig& config, std::unordered_map<ShaderType, ByteBuffer> shaderData)
		: ShaderProgram(config)
	{
		if (auto vertex = shaderData.find(ShaderType::Vertex); vertex != shaderData.end()) {
			mVertexShader = std::make_unique<ShaderVulkan>(graphics, ShaderType::Vertex, vertex->second);
		}
		if (auto fragment = shaderData.find(ShaderType::Fragment); fragment != shaderData.end()) {
			mFragmentShader = std::make_unique<ShaderVulkan>(graphics, ShaderType::Fragment, fragment->second);
		}
		if (auto compute = shaderData.find(ShaderType::Compute); compute != shaderData.end()) {
			mComputeShader = std::make_unique<ShaderVulkan>(graphics, ShaderType::Compute, compute->second);
		}
		_createVertexInputDesc();
	}

	Shader* ShaderProgramVulkan::GetShader(ShaderType type) const
	{
		switch (type) {
			case ShaderType::Vertex: return mVertexShader.get();
			case ShaderType::Fragment: return mFragmentShader.get();
			case ShaderType::Compute: return mComputeShader.get();
			default: ZE_FAIL("Shader type is not supported by Vulkan!");
		}
		return nullptr;
	}

	std::vector<vk::PipelineShaderStageCreateInfo> ShaderProgramVulkan::GetPipelineStageInfoVK() const
	{
		std::vector<vk::PipelineShaderStageCreateInfo> output;
		if (mVertexShader) {
			output.push_back(mVertexShader->GetPipelineStageInfoVK());
		}
		if (mFragmentShader) {
			output.push_back(mFragmentShader->GetPipelineStageInfoVK());
		}
		if (mComputeShader) {
			output.push_back(mComputeShader->GetPipelineStageInfoVK());
		}
		return output;
	}

	vk::PipelineVertexInputStateCreateInfo ShaderProgramVulkan::GetVertexInputDescriptionVK() const
	{
		return {
			vk::PipelineVertexInputStateCreateFlags(),
			static_cast<uint32_t>(mBindingDescList.size()),
			mBindingDescList.data(),
			static_cast<uint32_t>(mAttributeDescList.size()),
			mAttributeDescList.data(),
		};
	}

	void ShaderProgramVulkan::_createVertexInputDesc()
	{
		const auto& elements = GetVertexLayout();
		mAttributeDescList.reserve(elements.size());
		for (int i = 0; i < elements.size(); ++i) {
			const auto& element = elements[i];
			const vk::Format format = _getVertexAttributeFormat(element.mFormat);
			const vk::VertexInputRate inputRate = element.mIsInstanced ? vk::VertexInputRate::eInstance : vk::VertexInputRate::eVertex;

			if (mBindingDescList.size() == element.mSlot) {
				mBindingDescList.emplace_back(element.mSlot, 0, inputRate);
			}
			auto& bindingDesc = mBindingDescList[element.mSlot];
			ZE_ASSERT(bindingDesc.inputRate == inputRate, "Vulkan vertex element input rate does not match the bindings description!");

			mAttributeDescList.emplace_back(vk::VertexInputAttributeDescription(i, element.mSlot, format, bindingDesc.stride));
			bindingDesc.stride += VertexLayout::GetVertexFormatSize(element.mFormat);
		}
	}

	vk::Format ShaderProgramVulkan::_getVertexAttributeFormat(VertexFormat format) const
	{
		switch (format) {
			case VertexFormat::UInt: return vk::Format::eR32Uint;
			case VertexFormat::Float2: return vk::Format::eR32G32Sfloat;
			case VertexFormat::Float3: return vk::Format::eR32G32B32Sfloat;
			case VertexFormat::Float4: return vk::Format::eR32G32B32A32Sfloat;
			case VertexFormat::Color: return vk::Format::eR8G8B8A8Unorm;
			case VertexFormat::Unknown:
			default: ZE_FAIL("Vulkan vertex attribute format is not supported!");
		}
		return vk::Format::eUndefined;
	}
}
#endif