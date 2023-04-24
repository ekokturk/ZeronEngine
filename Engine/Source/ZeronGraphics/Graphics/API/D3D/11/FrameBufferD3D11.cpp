// Copyright (C) Eser Kokturk. All Rights Reserved.


#ifdef ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/FrameBufferD3D11.h>

#	include <d3d11.h>
#	include <Graphics/API/D3D/11/D3D11Helpers.h>
#	include <Graphics/API/D3D/11/SwapChainD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	FrameBufferD3D11::FrameBufferD3D11() {}

	void FrameBufferD3D11::CreateBuffers(ID3D11Device* device, SwapChainD3D11& swapChain, MSAALevel msaaLevel)
	{
		// Back Buffer
		ZE_D3D_ASSERT_RESULT(swapChain.GetSwapChainD3D()->GetBuffer(0, _uuidof(ID3D11Resource), &mBackBuffer));
		ZE_D3D_ASSERT_RESULT(device->CreateRenderTargetView(mBackBuffer.Get(), nullptr, &mRenderTargetView));

		// Depth Buffer
		const Vec2i size = swapChain.GetSize();
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = size.X;
		depthStencilDesc.Height = size.Y;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = D3D11Helpers::GetMultiSampleLevel(msaaLevel);
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		ZE_D3D_ASSERT_RESULT(device->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));
		ZE_D3D_ASSERT_RESULT(device->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf()));
	}

	void FrameBufferD3D11::ReleaseBuffers()
	{
		mBackBuffer.Reset();
		mRenderTargetView.Reset();
		mDepthStencilBuffer.Reset();
		mDepthStencilView.Reset();
	}

	ID3D11RenderTargetView* FrameBufferD3D11::GetRenderTargetD3D() const
	{
		return mRenderTargetView.Get();
	}

	ID3D11DepthStencilView* FrameBufferD3D11::GetDepthStencilD3D() const
	{
		return mDepthStencilView.Get();
	}
}
#endif