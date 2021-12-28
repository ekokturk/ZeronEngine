// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#ifdef ZE_GRAPHICS_D3D
#include "RenderTargetD3D11.h"
#include "SwapChainD3D11.h"
#include "GraphicsD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"
#include <d3d11.h>

namespace Zeron
{
	RenderTargetD3D11::RenderTargetD3D11(GraphicsD3D11& graphics, SwapChainD3D11& swapChain)
	{
		D3D_ASSERT_RESULT(swapChain.GetSwapChainD3D()->GetBuffer(0, _uuidof(ID3D11Resource), &mBackBuffer));
		D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateRenderTargetView(mBackBuffer.Get(), nullptr, &mRenderTargetView));

		const Vec2i windowSize = swapChain.GetWindowSize();
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = windowSize.X;
		depthStencilDesc.Height = windowSize.Y;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));
		D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf()));
	}
	
	ID3D11RenderTargetView* RenderTargetD3D11::GetRenderTargetD3D() const
	{
		return mRenderTargetView.Get();
	}

	ID3D11DepthStencilView* RenderTargetD3D11::GetDepthStencilD3D() const
	{
		return mDepthStencilView.Get();
	}
}
#endif
