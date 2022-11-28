// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/FrameBufferVulkan.h>

#include <Graphics/API/Vulkan/GraphicsVulkan.h>
#include <Graphics/API/Vulkan/RenderPassVulkan.h>
#include <Graphics/API/Vulkan/SwapChainVulkan.h>
#include <Graphics/API/Vulkan/TextureVulkan.h>

namespace Zeron
{
	FrameBufferVulkan::FrameBufferVulkan(
		GraphicsVulkan& graphics, 
		vk::Extent2D extent, 
		RenderPassVulkan& renderPass, 
		TextureVulkan* colorTexture, 
		TextureVulkan* depthTexture,
		TextureVulkan* samplingTexture
	)
		: mRenderPass(renderPass)
		, mExtent(extent)
	{
		const vk::Device& device = graphics.GetDeviceVK();

		std::vector<vk::ImageView> attachments;
		attachments.reserve(3);
		if (depthTexture) {
			mDepthImageView = depthTexture->CreateImageView(device, vk::ImageAspectFlagBits::eDepth);
			attachments.push_back(*mDepthImageView);
		}
		if (samplingTexture) {
			mSamplingImageView = samplingTexture->CreateImageView(device, vk::ImageAspectFlagBits::eColor);
			attachments.push_back(*mSamplingImageView);
		}
		if (colorTexture) {
			mColorImageView = colorTexture->CreateImageView(device, vk::ImageAspectFlagBits::eColor);
			attachments.push_back(*mColorImageView);
		}

		const vk::FramebufferCreateInfo info(
			vk::FramebufferCreateFlags(),
			mRenderPass.GetRenderPassVK(),
			attachments,
			mExtent.width,
			mExtent.height,
			1
		);
		mFrameBuffer = device.createFramebufferUnique(info);
	}

	FrameBufferVulkan::~FrameBufferVulkan()
	{
	}

	void FrameBufferVulkan::CreateBuffers(vk::PhysicalDevice& adapter, vk::Device& device, SwapChainVulkan& swapChain)
	{
	}

	void FrameBufferVulkan::ReleaseBuffers()
	{
	}

	vk::Framebuffer& FrameBufferVulkan::GetFrameBufferVK()
	{
        return *mFrameBuffer;
	}

	const vk::Extent2D& FrameBufferVulkan::GetExtentVK() const
	{
		return mExtent;
	}
}
#endif
