// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#ifdef ZE_GRAPHICS_D3D
#include "SwapChainD3D11.h"
#include "GraphicsD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"
#include "RenderTargetD3D11.h"
#include "Window/Window.h"
#include <d3d11.h>

namespace Zeron
{
	SwapChainD3D11::SwapChainD3D11(Window& window, GraphicsD3D11& graphics)
		: SwapChain(window)
		, mVSyncEnabled(0)
		, mHWND(nullptr)
		, mDeviceContext(graphics.GetDeviceContextD3D()) {

		mHWND = static_cast<HWND>(window.GetPlatformHandle());
		ZE_ASSERT(mHWND, "Win32 window handle returned null!");

		DXGI_SWAP_CHAIN_DESC desc;
		desc.BufferDesc.Width = window.GetWidth();
		desc.BufferDesc.Height = window.GetHeight();
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

		D3D_ASSERT_RESULT(graphics.GetFactoryD3D()->CreateSwapChain(graphics.GetDeviceD3D(), &desc, &mSwapChain));

		SetViewport(Vec2i::ZERO, window.GetSize());

		mRenderTarget = std::make_unique<RenderTargetD3D11>();
		mRenderTarget->CreateBuffers(graphics.GetDeviceD3D(), *this);
	}

	SwapChainD3D11::~SwapChainD3D11()
	{
	}

	void SwapChainD3D11::SetViewport(const Vec2i& pos, const Vec2i& size)
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
		D3D_ASSERT(mDeviceContext->RSSetViewports(1, &viewport));
	}

	void SwapChainD3D11::SwapBuffers()
	{
		D3D_ASSERT_RESULT(mSwapChain->Present(mVSyncEnabled, 0));
	}

	void SwapChainD3D11::SetVSyncEnabled(bool isEnabled)
	{
		mVSyncEnabled = isEnabled ? 1 : 0;
	}

	void SwapChainD3D11::SetRefreshRate(uint16_t rate)
	{
		// TODO
	}

	bool SwapChainD3D11::IsVSyncEnabled() const
	{
		return mVSyncEnabled == 0 ? false : true;
	}

	RenderTarget* SwapChainD3D11::GetRenderTarget() const
	{
		return mRenderTarget.get();
	}

	struct IDXGISwapChain* SwapChainD3D11::GetSwapChainD3D() const
	{
		return mSwapChain.Get();
	}
}
#endif
