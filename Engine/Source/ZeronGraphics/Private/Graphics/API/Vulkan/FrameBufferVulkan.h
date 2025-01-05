// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/FrameBuffer.h>

namespace Zeron::Gfx
{
	class Texture;
	class RenderPassVulkan;
	class TextureVulkan;
	class GraphicsVulkan;
	class SwapChainVulkan;

	class FrameBufferVulkan final : public FrameBuffer {
	  public:
		FrameBufferVulkan(
			GraphicsVulkan& graphics, RenderPassVulkan& renderPass, const Vec2i& extent, const std::span<Texture*>& colorTextures, Texture* depthTexture,
			const std::span<Texture*>& resolveTextures
		);
		~FrameBufferVulkan() override;

		RenderPassVulkan& GetRenderPass();

		// Vulkan
		vk::Framebuffer& GetFrameBufferVK();

	  private:
		RenderPassVulkan& mRenderPass;
		vk::UniqueFramebuffer mFrameBuffer;

		vk::UniqueImageView mDepthImageView;
		std::vector<vk::UniqueImageView> mColorImageViews;
		std::vector<vk::UniqueImageView> mResolveImageView;
	};
}
#endif