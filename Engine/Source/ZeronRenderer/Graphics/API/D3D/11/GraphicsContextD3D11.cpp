// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GraphicsContextD3D11.h"

#include "GraphicsD3D11.h"
#include "Window/Window.h"
#ifdef ZE_GRAPHICS_D3D
	#include "Graphics/API/D3D/DebugInfoD3D.h"
	#include <d3d11.h>
	#include <comdef.h>
	#include <initguid.h>

namespace Zeron {
	
	namespace WRL = Microsoft::WRL;
	
	GraphicsContextD3D11::GraphicsContextD3D11(Window* window, GraphicsD3D11& graphics)
		: GraphicsContext(window)
		, mGraphics(graphics)
		, mVSyncEnabled(0)
		, mHWND(nullptr) {

		ZE_ASSERT(window, "Window is null!");
		mHWND =	static_cast<HWND>(window->GetPlatformHandle());
		ZE_ASSERT(mHWND, "Win32 window handle returned null!");

		DXGI_SWAP_CHAIN_DESC desc;
		desc.BufferDesc.Width = window->GetWidth();					
		desc.BufferDesc.Height = window->GetHeight();					
		desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// Pixel layout
		desc.BufferDesc.RefreshRate.Numerator = 0;					// Get refresh rate of the window
		desc.BufferDesc.RefreshRate.Denominator = 0;				// Get refresh rate of the window
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		// Anti-aliasing (Set to none)
		desc.SampleDesc.Count = 1;	
		desc.SampleDesc.Quality = 0;
		
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// Buffer to be used as render target
		desc.BufferCount = 1;										// 1 back buffer
		desc.OutputWindow = mHWND;

		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// TODO: Investigate behavior

		D3D_ASSERT_RESULT(mGraphics.GetFactoryDXGI()->CreateSwapChain(mGraphics.GetDevice(), &desc, &mSwapChain));

		D3D_ASSERT_RESULT(mSwapChain->GetBuffer(0, _uuidof(ID3D11Resource), &mBackBuffer));
		
		D3D_ASSERT_RESULT(mGraphics.GetDevice()->CreateRenderTargetView(mBackBuffer.Get(), nullptr, &mRenderTarget));

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = window->GetWidth();
		depthStencilDesc.Height = window->GetHeight();
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D_ASSERT_RESULT(mGraphics.GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));
		D3D_ASSERT_RESULT(mGraphics.GetDevice()->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf()));
		
		SetRenderTarget();
		SetViewport(Vec2i::ZERO, window->GetSize());
	}

	void GraphicsContextD3D11::SetRenderTarget()
	{
		D3D_ASSERT(mGraphics.GetDeviceContext()->OMSetRenderTargets(1, mRenderTarget.GetAddressOf(), mDepthStencilView.Get()));
	}

	void GraphicsContextD3D11::ClearBuffer(Color color)
	{
		const float c[]{ color.normR(), color.normG(), color.normB(), color.normA() };
		mGraphics.GetDeviceContext()->ClearRenderTargetView(mRenderTarget.Get(), c);
		mGraphics.GetDeviceContext()->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void GraphicsContextD3D11::SetViewport(const Vec2i& pos, const Vec2i& size)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = static_cast<FLOAT>(pos.X);
		viewport.TopLeftY = static_cast<FLOAT>(pos.Y);
		viewport.Width = static_cast<FLOAT>(size.X);
		viewport.Height = static_cast<FLOAT>(size.Y);
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		
		// TODO: Multiple viewports
		D3D_ASSERT(mGraphics.GetDeviceContext()->RSSetViewports(1, &viewport));
	}

	void GraphicsContextD3D11::SwapBuffers()
	{
		D3D_ASSERT_RESULT(mSwapChain->Present(mVSyncEnabled, 0));
	}

	void GraphicsContextD3D11::SetVSyncEnabled(bool isEnabled)
	{
		mVSyncEnabled = isEnabled ? 1 : 0;
	}

	void GraphicsContextD3D11::SetRefreshRate(uint16_t rate)
	{
		// TODO
	}

	bool GraphicsContextD3D11::IsVSyncEnabled() const
	{
		return mVSyncEnabled == 0 ? false : true;
	}

	ID3D11RenderTargetView* GraphicsContextD3D11::GetRenderTarget() const
	{
		return mRenderTarget.Get();
	}
}
#endif
