// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/RenderPassVulkan.h>

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>

namespace Zeron::Gfx
{
	RenderPassVulkan::RenderPassVulkan(GraphicsVulkan& graphics, vk::Format colorFormat, vk::Format depthFormat, vk::SampleCountFlagBits sampling)
	{
		std::vector<vk::AttachmentDescription> attachmentDescriptions;
		const bool hasMSAA = sampling != vk::SampleCountFlagBits::e1;
		const bool hasColorAttachment = colorFormat != vk::Format::eUndefined;
		const bool hasDepthAttachment = depthFormat != vk::Format::eUndefined;

		vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
		vk::AttachmentReference resolveReference(0, vk::ImageLayout::eColorAttachmentOptimal);
		vk::AttachmentReference depthReference(0, vk::ImageLayout::eDepthStencilAttachmentOptimal);

		// Depth
		if (hasDepthAttachment) {
			attachmentDescriptions.emplace_back(
				vk::AttachmentDescriptionFlags(),
				depthFormat,
				hasMSAA ? sampling : vk::SampleCountFlagBits::e1,
				vk::AttachmentLoadOp::eClear,
				vk::AttachmentStoreOp::eDontCare,
				vk::AttachmentLoadOp::eDontCare,
				vk::AttachmentStoreOp::eDontCare,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::eDepthStencilAttachmentOptimal
			);
			depthReference.attachment = static_cast<uint32_t>(attachmentDescriptions.size() - 1);
		}

		if (hasColorAttachment) {
			// Sampling
			if (hasMSAA) {
				attachmentDescriptions.emplace_back(
					vk::AttachmentDescriptionFlags(),
					colorFormat,
					sampling,
					vk::AttachmentLoadOp::eClear,
					vk::AttachmentStoreOp::eDontCare,
					vk::AttachmentLoadOp::eDontCare,
					vk::AttachmentStoreOp::eDontCare,
					vk::ImageLayout::eUndefined,
					hasMSAA ? vk::ImageLayout::eColorAttachmentOptimal : vk::ImageLayout::ePresentSrcKHR
				);
				colorReference.attachment = static_cast<uint32_t>(attachmentDescriptions.size() - 1);
				resolveReference.attachment = static_cast<uint32_t>(attachmentDescriptions.size());
			}
			else {
				colorReference.attachment = static_cast<uint32_t>(attachmentDescriptions.size());
			}

			// Color
			attachmentDescriptions.emplace_back(vk::AttachmentDescription(
				vk::AttachmentDescriptionFlags(),
				colorFormat,
				vk::SampleCountFlagBits::e1,
				hasMSAA ? vk::AttachmentLoadOp::eDontCare : vk::AttachmentLoadOp::eClear,
				vk::AttachmentStoreOp::eStore,
				vk::AttachmentLoadOp::eDontCare,
				vk::AttachmentStoreOp::eDontCare,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::ePresentSrcKHR
			));
		}

		const vk::SubpassDescription subpass(
			vk::SubpassDescriptionFlags(),
			vk::PipelineBindPoint::eGraphics,
			0,
			nullptr,
			hasColorAttachment ? 1 : 0,
			hasColorAttachment ? &colorReference : nullptr,
			hasMSAA ? &resolveReference : nullptr,
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

		const vk::RenderPassCreateInfo createInfo(vk::RenderPassCreateFlags(), attachmentDescriptions, subpass);

		mRenderPass = graphics.GetDeviceVK().createRenderPassUnique(createInfo);
	}

	RenderPassVulkan::~RenderPassVulkan() {}

	vk::RenderPass& RenderPassVulkan::GetRenderPassVK()
	{
		return *mRenderPass;
	}
}

#endif