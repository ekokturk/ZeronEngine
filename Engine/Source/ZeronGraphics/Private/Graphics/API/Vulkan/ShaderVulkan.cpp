// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/ShaderVulkan.h>

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron::Gfx
{
	ShaderVulkan::ShaderVulkan(GraphicsVulkan& graphics, ShaderType type, const ByteBuffer& buffer)
		: Shader(type)
		, mBuffer(buffer)
	{
		const vk::ShaderModuleCreateInfo createInfo(vk::ShaderModuleCreateFlags(), mBuffer.size(), reinterpret_cast<uint32_t*>(mBuffer.data()));
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

}
#endif