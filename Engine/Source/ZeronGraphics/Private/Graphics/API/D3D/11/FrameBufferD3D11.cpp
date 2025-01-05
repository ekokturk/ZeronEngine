// Copyright (C) Eser Kokturk. All Rights Reserved.


#include <Graphics/API/D3D/11/GraphicsD3D11.h>
#ifdef ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/FrameBufferD3D11.h>

#	include <d3d11.h>
#	include <Graphics/API/D3D/11/D3D11Helpers.h>
#	include <Graphics/API/D3D/11/RenderPassD3D11.h>
#	include <Graphics/API/D3D/11/TextureD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	FrameBufferD3D11::FrameBufferD3D11(
		GraphicsD3D11& graphics, RenderPassD3D11& renderPass, const Vec2i& extent, const std::span<Texture*>& colorTextures, Texture* depthTexture,
		const std::span<Texture*>& resolveTextures
	)
		: FrameBuffer(extent)
	{
		ID3D11Device& device = *graphics.GetDeviceD3D();
		const std::vector<RenderPassAttachment>& colorAttachments = renderPass.GetColorAttachments();
		ZE_ASSERT(colorTextures.size() == colorAttachments.size(), "FrameBufferD3D11: Expected to have same amount of color attachmtents defined in the render pass.");

		if (const RenderPassAttachment* depthAttachment = renderPass.GetDepthAttachment()) {
			ZE_ASSERT(depthAttachment->mFormat == depthAttachment->mFormat, "FrameBufferD3D11: Depth texture format needs to match render pass.");
			ZE_ASSERT(depthTexture, "FrameBufferD3D11: Expected depth texture to exist.");
			mDepthView = static_cast<TextureD3D11*>(depthTexture)->CreateDepthStencilViewD3D(device);
		}

		for (int i = 0; i < colorAttachments.size(); ++i) {
			ZE_ASSERT(colorTextures[i], "FrameBufferD3D11: Color texture cannot be null.");
			const RenderPassAttachment& colorAttachment = colorAttachments[i];
			const auto& colorTexture = static_cast<const TextureD3D11&>(*colorTextures[i]);
			ZE_ASSERT(colorAttachment.mFormat == colorTexture.GetFormat(), "FrameBufferD3D11: Color texture format needs to match render pass.");
			mRenderTargetViews.push_back(colorTexture.CreateRenderTargetViewD3D(device));
			mRenderTargetViewBinding.push_back(mRenderTargetViews.back().Get());
		}
	}

	FrameBufferD3D11::~FrameBufferD3D11()
	{
		mDepthView = nullptr;
		mRenderTargetViewBinding.clear();
		mRenderTargetViews.clear();
	}

	void FrameBufferD3D11::BindD3D(ID3D11DeviceContext* ctx, Color clearColor)
	{
		ZE_D3D_ASSERT(ctx->OMSetRenderTargets(mRenderTargetViewBinding.size(), mRenderTargetViewBinding.empty() ? nullptr : mRenderTargetViewBinding.data(), mDepthView.Get())
		);
		const std::array color{ clearColor.normR(), clearColor.normG(), clearColor.normB(), clearColor.normA() };
		for (const auto binding : mRenderTargetViewBinding) {
			ZE_D3D_ASSERT(ctx->ClearRenderTargetView(binding, color.data()));
		}
		if (mDepthView) {
			ZE_D3D_ASSERT(ctx->ClearDepthStencilView(mDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0));
		}
	}
	void FrameBufferD3D11::UnbindD3D(ID3D11DeviceContext* ctx)
	{
		ZE_D3D_ASSERT(ctx->OMSetRenderTargets(0, nullptr, nullptr));
	}


}
#endif