// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/GraphicsTypes.h>
#	include <Graphics/SwapChain.h>

struct IDXGISwapChain;

namespace Zeron::Gfx
{
	class RenderPassD3D11;
	class TextureD3D11;
	class GraphicsD3D11;
	class FrameBufferD3D11;
	class Window;

	class SwapChainD3D11 final : public SwapChain {
	  public:
		SwapChainD3D11(GraphicsD3D11& graphics, SystemHandle systemHandle, const Vec2i& size);
		~SwapChainD3D11();

		void Present() override;
		void SetVSyncEnabled(bool isEnabled) override;
		void SetRefreshRate(uint16_t rate) override;

		bool IsVSyncEnabled() const override;
		FrameBuffer* GetFrameBuffer() const override;

		// D3D11
		IDXGISwapChain* GetSwapChainD3D() const;
		void Resize(const Vec2i& size);

	  private:
		void _createFrameBuffer();
		void _releaseFrameBuffer();

		GraphicsD3D11& mGraphics;
		std::unique_ptr<RenderPassD3D11> mSwapChainRenderPass;
		std::unique_ptr<FrameBufferD3D11> mFrameBuffer;
		Gfx::ComPtr<IDXGISwapChain> mSwapChain;
		UINT mVSyncEnabled;
		HWND mHWND;

		std::unique_ptr<TextureD3D11> mDepthTexture;
		std::unique_ptr<TextureD3D11> mColorTexture;
	};
}
#endif