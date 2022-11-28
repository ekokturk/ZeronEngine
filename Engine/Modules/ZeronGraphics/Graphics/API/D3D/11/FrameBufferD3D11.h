// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/FrameBuffer.h>

#include <Graphics/GraphicsTypes.h>
#include <Graphics/API/D3D/DXGI.h>

struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

namespace Zeron
{
	class GraphicsD3D11;
	class SwapChainD3D11;

	class FrameBufferD3D11 : public FrameBuffer {
	public:
		FrameBufferD3D11();

		void CreateBuffers(ID3D11Device* device, SwapChainD3D11& swapChain, MSAALevel msaaLevel);
		void ReleaseBuffers();
		
		ID3D11RenderTargetView* GetRenderTargetD3D() const;
		ID3D11DepthStencilView* GetDepthStencilD3D() const;
	
	private:
		
		ZE::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		ZE::ComPtr<ID3D11Texture2D> mBackBuffer;

		ZE::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		ZE::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	};
}
#endif
