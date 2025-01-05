// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{

	struct RenderPassAttachment {
		TextureFormat mFormat = TextureFormat::Undefined;
		TextureLayout mOutLayout = TextureLayout::Undefined;
		TextureLayout mInLayout = TextureLayout::Undefined;
	};

	class RenderPass {
	  public:
		virtual ~RenderPass() = default;

		const std::vector<RenderPassAttachment>& GetColorAttachments() const;
		const RenderPassAttachment* GetDepthAttachment() const;
		bool HasDepthAttachment() const;
		bool HasSampling() const;
		MSAALevel GetSampling() const;

	  protected:
		RenderPass(std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling);

		std::vector<RenderPassAttachment> mColorAttachments;
		std::optional<RenderPassAttachment> mDepthAttachment;
		MSAALevel mSampling;
	};
}