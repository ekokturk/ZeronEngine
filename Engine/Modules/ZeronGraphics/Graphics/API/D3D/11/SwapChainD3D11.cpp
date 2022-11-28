// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/API/D3D/11/SwapChainD3D11.h>

#ifdef ZE_GRAPHICS_D3D
#include <d3d11.h>
#include <Graphics/API/D3D/11/GraphicsD3D11.h>
#include <Graphics/API/D3D/11/FrameBufferD3D11.h>
#include <Graphics/API/D3D/11/D3D11Helpers.h>
#include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron
{
	SwapChainD3D11::SwapChainD3D11(GraphicsD3D11& graphics, SystemHandle systemHandle, const Vec2i& size)
		: SwapChain(size, 2)
		, mVSyncEnabled(0)
		, mHWND(nullptr) {

		mHWND = static_cast<HWND>(systemHandle.mWindow);
		ZE_ASSERT(mHWND, "Win32 window handle returned null!");

		const Vec2i& windowSize = GetSize();
		const MSAALevel msaaLevel = graphics.GetMultiSamplingLevel();

		DXGI_SWAP_CHAIN_DESC desc;
		desc.BufferDesc.Width = windowSize.X;
		desc.BufferDesc.Height = windowSize.Y;
		desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// Pixel layout
		desc.BufferDesc.RefreshRate.Numerator = 0;					// Get refresh rate of the window
		desc.BufferDesc.RefreshRate.Denominator = 0;				// Get refresh rate of the window
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		// Anti-aliasing (Set to none)
		const UINT sampleCount = D3D11Helpers::GetMultiSampleLevel(msaaLevel);
		// TODO: Use sample quality for MSAA
		//UINT sampleQuality = 0;
		//graphics.GetDeviceD3D()->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, sampleCount, &sampleQuality);
		desc.SampleDesc.Count = sampleCount;
		desc.SampleDesc.Quality = 0;

		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// Buffer to be used as render target
		desc.BufferCount = GetFrameCount() - 1;				// Back buffer count
		desc.OutputWindow = mHWND;

		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// TODO: Investigate behavior

		D3D_ASSERT_RESULT(graphics.GetFactoryD3D()->CreateSwapChain(graphics.GetDeviceD3D(), &desc, &mSwapChain));

		mFrameBuffer = std::make_unique<FrameBufferD3D11>();
		mFrameBuffer->CreateBuffers(graphics.GetDeviceD3D(), *this, msaaLevel);
	}

	SwapChainD3D11::~SwapChainD3D11()
	{
	}

	void SwapChainD3D11::Present()
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

	void SwapChainD3D11::Resize(const Vec2i& size)
	{
		ZE_ASSERT(!mFrameBuffer->GetRenderTargetD3D(), "Render target needs to be released before we resize the swap chain!");
		ZE_ASSERT(!mFrameBuffer->GetDepthStencilD3D(), "Depth Stencil needs to be released before we resize the swap chain!");
		_setSize(size);
		D3D_ASSERT_RESULT(mSwapChain->ResizeBuffers(0, size.X, size.Y, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
	}

	bool SwapChainD3D11::IsVSyncEnabled() const
	{
		return mVSyncEnabled == 0 ? false : true;
	}

	FrameBuffer* SwapChainD3D11::GetFrameBuffer() const
	{
		return mFrameBuffer.get();
	}

	struct IDXGISwapChain* SwapChainD3D11::GetSwapChainD3D() const
	{
		return mSwapChain.Get();
	}
}
#endif
