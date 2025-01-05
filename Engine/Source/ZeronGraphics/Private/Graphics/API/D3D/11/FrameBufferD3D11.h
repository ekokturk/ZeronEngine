// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/FrameBuffer.h>

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/GraphicsTypes.h>

struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

namespace Zeron::Gfx
{
	class Texture;
	class RenderPassD3D11;
	class GraphicsD3D11;
	class SwapChainD3D11;
	class TextureD3D11;

	class FrameBufferD3D11 : public FrameBuffer {
	  public:
		FrameBufferD3D11(
			GraphicsD3D11& graphics, RenderPassD3D11& renderPass, const Vec2i& extent, const std::span<Texture*>& colorTextures, Texture* depthTexture,
			const std::span<Texture*>& resolveTextures
		);
		~FrameBufferD3D11();

		void BindD3D(ID3D11DeviceContext* ctx, Color clearColor);
		void UnbindD3D(ID3D11DeviceContext* ctx);

	  private:
		std::vector<ID3D11RenderTargetView*> mRenderTargetViewBinding;
		std::vector<Gfx::ComPtr<ID3D11RenderTargetView>> mRenderTargetViews;
		Gfx::ComPtr<ID3D11DepthStencilView> mDepthView;
	};
}
#endif