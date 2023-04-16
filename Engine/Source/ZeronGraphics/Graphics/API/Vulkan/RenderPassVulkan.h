// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/RenderPass.h>

namespace Zeron
{
	class GraphicsVulkan;

	class RenderPassVulkan final : public RenderPass {
	  public:
		RenderPassVulkan(GraphicsVulkan& graphics, vk::Format colorFormat, vk::Format depthFormat, vk::SampleCountFlagBits sampling);
		~RenderPassVulkan();

		// Vulkan
		vk::RenderPass& GetRenderPassVK();

	  private:
		vk::UniqueRenderPass mRenderPass;
	};

}
#endif