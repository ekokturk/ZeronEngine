// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/GraphicsTypes.h>
#	include <Graphics/RenderPass.h>

namespace Zeron::Gfx
{
	class GraphicsD3D11;

	class RenderPassD3D11 : public RenderPass {
	  public:
		RenderPassD3D11(GraphicsD3D11& graphics, std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling);
	};
}
#endif