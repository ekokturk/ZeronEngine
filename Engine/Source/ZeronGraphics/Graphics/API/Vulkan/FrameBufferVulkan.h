// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/FrameBuffer.h>

namespace Zeron::Gfx
{
	class RenderPassVulkan;
	class TextureVulkan;
	class GraphicsVulkan;
	class SwapChainVulkan;

	class FrameBufferVulkan final : public FrameBuffer {
	  public:
		FrameBufferVulkan(
			GraphicsVulkan& graphics, vk::Extent2D extent, RenderPassVulkan& renderPass, TextureVulkan* colorView, TextureVulkan* depthView, TextureVulkan* samplingView
		);
		~FrameBufferVulkan();

		void CreateBuffers(vk::PhysicalDevice& adapter, vk::Device& device, SwapChainVulkan& swapChain);
		void ReleaseBuffers();

		// Vulkan
		vk::Framebuffer& GetFrameBufferVK();
		const vk::Extent2D& GetExtentVK() const;

	  private:
		vk::UniqueFramebuffer mFrameBuffer;

		// TODO: Remove this?
		RenderPassVulkan& mRenderPass;

		vk::UniqueImageView mColorImageView;
		vk::UniqueImageView mDepthImageView;
		vk::UniqueImageView mSamplingImageView;

		vk::Extent2D mExtent;
	};
}
#endif