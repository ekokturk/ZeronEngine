// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/FrameBufferVulkan.h>

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/RenderPassVulkan.h>
#	include <Graphics/API/Vulkan/TextureVulkan.h>

namespace Zeron::Gfx
{
	FrameBufferVulkan::FrameBufferVulkan(
		GraphicsVulkan& graphics, RenderPassVulkan& renderPass, const Vec2i& extent, const std::span<Texture*>& colorTextures, Texture* depthTexture,
		const std::span<Texture*>& resolveTextures
	)
		: FrameBuffer(extent)
		, mRenderPass(renderPass)
	{
		const vk::Device& device = graphics.GetDeviceVK();
		std::vector<vk::ImageView> attachments;
		const std::vector<RenderPassAttachment>& colorAttachments = mRenderPass.GetColorAttachments();
		ZE_ASSERT(colorTextures.size() == colorAttachments.size(), "VulkanFrameBuffer: Expected to have same amount of color attachmtents defined in the render pass.");

		if (const RenderPassAttachment* depthAttachment = renderPass.GetDepthAttachment()) {
			ZE_ASSERT(depthAttachment->mFormat == depthAttachment->mFormat, "VulkanFrameBuffer: Depth texture format needs to match render pass.");
			mDepthImageView = static_cast<TextureVulkan*>(depthTexture)->CreateImageView(device, vk::ImageAspectFlagBits::eDepth);
			attachments.push_back(*mDepthImageView);
		}

		for (int i = 0; i < colorAttachments.size(); ++i) {
			ZE_ASSERT(colorTextures[i], "VulkanFrameBuffer: Color texture cannot be null.");
			const RenderPassAttachment& colorAttachment = colorAttachments[i];
			const TextureVulkan& colorTexture = static_cast<const TextureVulkan&>(*colorTextures[i]);
			ZE_ASSERT(colorAttachment.mFormat == colorTexture.GetFormat(), "VulkanFrameBuffer: Color texture format needs to match render pass.");
			mColorImageViews.emplace_back(colorTexture.CreateImageView(device, vk::ImageAspectFlagBits::eColor));
			attachments.push_back(*mColorImageViews.back());

			if (mRenderPass.HasSampling()) {
				const TextureVulkan& resolveTexture = static_cast<const TextureVulkan&>(*resolveTextures[i]);
				ZE_ASSERT(colorAttachment.mFormat == resolveTexture.GetFormat(), "VulkanFrameBuffer: Color texture format needs to match render pass.");
				mResolveImageView.emplace_back(resolveTexture.CreateImageView(device, vk::ImageAspectFlagBits::eColor));
				attachments.push_back(*mResolveImageView.back());
			}
		}

		const vk::FramebufferCreateInfo info(vk::FramebufferCreateFlags(), mRenderPass.GetRenderPassVK(), attachments.size(), attachments.data(), extent.X, extent.Y, 1);
		mFrameBuffer = device.createFramebufferUnique(info);
	}

	FrameBufferVulkan::~FrameBufferVulkan()
	{
		mDepthImageView.reset();
		mColorImageViews.clear();
		mResolveImageView.clear();
	}


	vk::Framebuffer& FrameBufferVulkan::GetFrameBufferVK()
	{
		return *mFrameBuffer;
	}

	RenderPassVulkan& FrameBufferVulkan::GetRenderPass()
	{
		return mRenderPass;
	}

}
#endif