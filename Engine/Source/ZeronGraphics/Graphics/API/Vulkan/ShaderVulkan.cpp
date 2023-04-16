// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/ShaderVulkan.h>

#include <Graphics/VertexLayout.h>
#include <Graphics/API/Vulkan/GraphicsVulkan.h>
#include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron
{
	ShaderVulkan::ShaderVulkan(GraphicsVulkan& graphics, ShaderType type, const ByteBuffer& buffer)
		: Shader(type)
		, mBuffer(buffer)
	{
		const vk::ShaderModuleCreateInfo createInfo(
			vk::ShaderModuleCreateFlags(),
			mBuffer.size(),
			reinterpret_cast<uint32_t*>(mBuffer.data())
		);
		mShader = graphics.GetDeviceVK().createShaderModuleUnique(createInfo);

		mPipelineStageInfo.flags = vk::PipelineShaderStageCreateFlags();
		mPipelineStageInfo.module = mShader.get();
		mPipelineStageInfo.pName = "main";
		mPipelineStageInfo.stage = VulkanHelpers::GetShaderStage(mType);
	}

	const vk::PipelineShaderStageCreateInfo& ShaderVulkan::GetPipelineStageInfoVK() const
	{
		return mPipelineStageInfo;
	}

	ShaderProgramVulkan::ShaderProgramVulkan(GraphicsVulkan& graphics, const std::string& shaderName, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout,
		const ByteBuffer& vertexShader, const ByteBuffer& fragmentShader, const ByteBuffer& computeShader)
		: ShaderProgram(shaderName, vertexLayout, resourceLayout)
	{
		if (!vertexShader.empty()) {
			mVertexShader = std::make_unique<ShaderVulkan>(graphics, ShaderType::Vertex, vertexShader);
		}
		if (!fragmentShader.empty()) {
			mFragmentShader = std::make_unique<ShaderVulkan>(graphics, ShaderType::Fragment, fragmentShader);
		}
		if (!computeShader.empty()) {
			mComputeShader = std::make_unique<ShaderVulkan>(graphics, ShaderType::Compute, computeShader);
		}
		_createVertexInputDesc();
	}

	ShaderProgramVulkan::ShaderProgramVulkan(GraphicsVulkan& graphics, const std::string& shaderName, const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout)
		: ShaderProgram(shaderName, vertexLayout, resourceLayout)
	{
		mVertexShader = std::static_pointer_cast<ShaderVulkan>(vertexShader);
		mFragmentShader = std::static_pointer_cast<ShaderVulkan>(fragmentShader);
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
			mBindingDescList,
			mAttributeDescList
		};
	}

	void ShaderProgramVulkan::_createVertexInputDesc()
	{
		const auto& elements = mVertexLayout.GetElements();
		mAttributeDescList.reserve(elements.size());
		for (int i = 0; i < elements.size(); ++i) {
			const auto& element = elements[i];
			const vk::Format format = _getVertexAttributeFormat(element.mFormat);
			const vk::VertexInputRate inputRate = element.mIsInstanced ? vk::VertexInputRate::eInstance : vk::VertexInputRate::eVertex;

			if (mBindingDescList.size() == element.mSlot) {
				mBindingDescList.emplace_back(
					element.mSlot,
					0,
					inputRate
				);
			}
			auto& bindingDesc = mBindingDescList[element.mSlot];
			ZE_ASSERT(bindingDesc.inputRate == inputRate, "Vulkan vertex element input rate does not match the bindings description!");

			mAttributeDescList.emplace_back(
				vk::VertexInputAttributeDescription(
					i,
					element.mSlot,
					format,
					bindingDesc.stride
				)
			);
			bindingDesc.stride += VertexLayout::GetVertexFormatSize(element.mFormat);
		}
	}

	vk::Format ShaderProgramVulkan::_getVertexAttributeFormat(VertexFormat format) const
	{
		switch (format) {
			case VertexFormat::Float2: return vk::Format::eR32G32Sfloat;
			case VertexFormat::Float3: return vk::Format::eR32G32B32Sfloat;
			case VertexFormat::Color: return vk::Format::eR8G8B8A8Unorm;
			case VertexFormat::Unknown:
			default: ZE_FAIL("Vulkan vertex attribute format is not supported!");
		}
		return vk::Format::eUndefined;
	}
}
#endif
