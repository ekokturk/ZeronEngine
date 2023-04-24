// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/GraphicsTypes.h>
#	include <Graphics/SwapChain.h>

namespace Zeron::Gfx
{
	class TextureVulkan;
	class RenderPass;
	class RenderPassVulkan;
	class FrameBufferVulkan;
	class GraphicsVulkan;

	class SwapChainVulkan final : public SwapChain {
	  public:
		SwapChainVulkan(GraphicsVulkan& graphics, SystemHandle systemHandle, const Vec2i& size, vk::UniqueSurfaceKHR surface);
		~SwapChainVulkan();

		// SwapChain
		void Present() override;
		void SetVSyncEnabled(bool isEnabled) override;
		void SetRefreshRate(uint16_t rate) override;
		bool IsVSyncEnabled() const override;
		FrameBuffer* GetFrameBuffer() const override;

		// TODO: Add these to the base class
		RenderPass& GetRenderPass();
		uint32_t GetCurrentFrameIndex() const;

		// Vulkan API
		vk::SwapchainKHR& GetSwapChainVK();
		vk::Format GetColorFormatVK() const;
		vk::Extent2D GetExtendVK() const;

		void AcquireNextFrame(const vk::Device& device, vk::Semaphore semaphore);
		void Recreate(GraphicsVulkan& graphics, const Vec2i& size);
		void Present(GraphicsVulkan& graphics, vk::Semaphore semaphore);

	  private:
		void _initSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		void _createSwapChain(GraphicsVulkan& graphics, vk::SwapchainKHR oldSwapChain);

		vk::UniqueSurfaceKHR mSurface;
		vk::UniqueSwapchainKHR mSwapChain;

		std::vector<TextureVulkan> mColorTextures;
		std::unique_ptr<TextureVulkan> mDepthTexture;
		std::unique_ptr<TextureVulkan> mSamplingTexture;

		SystemHandle mSystemHandle;
		uint32_t mPreferredFrameCount;
		uint32_t mCurrentFrameIndex;

		std::vector<std::unique_ptr<FrameBufferVulkan>> mSwapChainFrameBuffers;
		std::unique_ptr<RenderPassVulkan> mSwapChainRenderPass;

		// Depth pre-pass resources
		std::unique_ptr<FrameBufferVulkan> mPreDepthFrameBuffer;
		std::unique_ptr<RenderPassVulkan> mPreDepthRenderPass;

		vk::Format mColorFormat;
		vk::ColorSpaceKHR mColorSpace;
		vk::Format mDepthFormat;

		vk::PresentModeKHR mPresentMode;
		vk::CompositeAlphaFlagBitsKHR mCompositeAlpha;
		vk::SurfaceTransformFlagBitsKHR mSurfaceTransform;
	};
}
#endif