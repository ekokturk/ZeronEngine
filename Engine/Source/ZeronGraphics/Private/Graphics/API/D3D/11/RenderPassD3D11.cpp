// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/RenderPassD3D11.h>

#	include <d3d11.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	RenderPassD3D11::RenderPassD3D11(
		GraphicsD3D11& graphics, std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling
	)
		: RenderPass(std::move(colorAttachments), depthAttachment, sampling)
	{}

}
#endif