// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/RenderPass.h>

namespace Zeron::Gfx
{
	class GraphicsVulkan;

	class RenderPassVulkan final : public RenderPass {
	  public:
		RenderPassVulkan(
			GraphicsVulkan& graphics, std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling
		);
		~RenderPassVulkan();

		// Vulkan
		vk::RenderPass& GetRenderPassVK();

	  private:
		vk::UniqueRenderPass mRenderPass;
	};

}
#endif