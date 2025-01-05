// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/RenderPassVulkan.h>

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/VulkanHelpers.h>

namespace Zeron::Gfx
{
	RenderPassVulkan::RenderPassVulkan(
		GraphicsVulkan& graphics, std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling
	)
		: RenderPass(std::move(colorAttachments), depthAttachment, sampling)
	{
		std::vector<vk::AttachmentDescription> attachmentDescriptions;
		const bool hasMSAA = sampling != MSAALevel::Disabled;
		const bool hasDepthAttachment = mDepthAttachment.has_value() && mDepthAttachment->mFormat != TextureFormat::Undefined;

		vk::AttachmentReference depthReference(0, vk::ImageLayout::eDepthStencilAttachmentOptimal);
		std::vector<vk::AttachmentReference> colorReferences(mColorAttachments.size(), { 0, vk::ImageLayout::eColorAttachmentOptimal });
		std::vector<vk::AttachmentReference> resolveReferences;

		// Depth
		if (hasDepthAttachment) {
			attachmentDescriptions.emplace_back(
				vk::AttachmentDescriptionFlags(),
				VulkanHelpers::GetTextureFormat(mDepthAttachment->mFormat),
				VulkanHelpers::GetMultiSamplingLevel(mSampling),
				vk::AttachmentLoadOp::eClear,
				vk::AttachmentStoreOp::eDontCare,
				vk::AttachmentLoadOp::eDontCare,
				vk::AttachmentStoreOp::eDontCare,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::eDepthStencilAttachmentOptimal
			);
			depthReference.attachment = static_cast<uint32_t>(attachmentDescriptions.size() - 1);
		}

		for (size_t i = 0; i < mColorAttachments.size(); ++i) {
			const RenderPassAttachment& colorAttachment = mColorAttachments[i];

			vk::ImageLayout finalLayout = colorAttachment.mOutLayout == TextureLayout::Present && !hasMSAA ? vk::ImageLayout::ePresentSrcKHR :
																											 vk::ImageLayout::eColorAttachmentOptimal;
			attachmentDescriptions.emplace_back(
				vk::AttachmentDescriptionFlags(),
				VulkanHelpers::GetTextureFormat(colorAttachment.mFormat),
				VulkanHelpers::GetMultiSamplingLevel(mSampling),
				vk::AttachmentLoadOp::eClear,
				hasMSAA ? vk::AttachmentStoreOp::eDontCare : vk::AttachmentStoreOp::eStore,
				vk::AttachmentLoadOp::eDontCare,
				vk::AttachmentStoreOp::eDontCare,
				vk::ImageLayout::eUndefined,
				finalLayout
			);
			colorReferences[i].attachment = static_cast<uint32_t>(attachmentDescriptions.size() - 1);

			if (hasMSAA) {
				attachmentDescriptions.emplace_back(
					vk::AttachmentDescriptionFlags(),
					VulkanHelpers::GetTextureFormat(colorAttachment.mFormat),
					vk::SampleCountFlagBits::e1,
					hasMSAA ? vk::AttachmentLoadOp::eDontCare : vk::AttachmentLoadOp::eClear,
					vk::AttachmentStoreOp::eStore,
					vk::AttachmentLoadOp::eDontCare,
					vk::AttachmentStoreOp::eDontCare,
					vk::ImageLayout::eUndefined,
					colorAttachment.mOutLayout == TextureLayout::Present ? vk::ImageLayout::ePresentSrcKHR : vk::ImageLayout::eUndefined
				);
				resolveReferences.emplace_back(static_cast<uint32_t>(attachmentDescriptions.size() - 1), vk::ImageLayout::eColorAttachmentOptimal);
			}
		}

		std::vector<vk::SubpassDescription> subPassDescriptions;
		subPassDescriptions.emplace_back(
			vk::SubpassDescriptionFlags(),
			vk::PipelineBindPoint::eGraphics,
			0,
			nullptr,
			static_cast<uint32_t>(colorReferences.size()),
			!colorReferences.empty() ? colorReferences.data() : nullptr,
			!resolveReferences.empty() ? resolveReferences.data() : nullptr,
			hasDepthAttachment ? &depthReference : nullptr,
			0,
			nullptr
		);

		// const vk::SubpassDependency subpassDependency{
		//	0,
		//	0,
		//	vk::PipelineStageFlagBits::eColorAttachmentOutput,
		//	vk::PipelineStageFlagBits::eColorAttachmentOutput,
		//	vk::AccessFlags(),
		//	vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
		//	vk::DependencyFlags()
		// };

		const vk::RenderPassCreateInfo createInfo(
			vk::RenderPassCreateFlags(),
			static_cast<uint32_t>(attachmentDescriptions.size()),
			attachmentDescriptions.data(),
			static_cast<uint32_t>(subPassDescriptions.size()),
			subPassDescriptions.data()
		);

		mRenderPass = graphics.GetDeviceVK().createRenderPassUnique(createInfo);
	}

	RenderPassVulkan::~RenderPassVulkan() {}

	vk::RenderPass& RenderPassVulkan::GetRenderPassVK()
	{
		return *mRenderPass;
	}
}

#endif