// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/RenderPass.h>

namespace Zeron::Gfx
{

	RenderPass::RenderPass(std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling)
		: mColorAttachments(std::move(colorAttachments))
		, mDepthAttachment(depthAttachment)
		, mSampling(sampling)
	{}

	const std::vector<RenderPassAttachment>& RenderPass::GetColorAttachments() const
	{
		return mColorAttachments;
	}

	const RenderPassAttachment* RenderPass::GetDepthAttachment() const
	{
		return mDepthAttachment.has_value() ? &(*mDepthAttachment) : nullptr;
	}

	bool RenderPass::HasDepthAttachment() const
	{
		return mDepthAttachment.has_value();
	}

	bool RenderPass::HasSampling() const
	{
		return mSampling != MSAALevel::Disabled;
	}

	MSAALevel RenderPass::GetSampling() const
	{
		return mSampling;
	}
}