// Copyright (C) Eser Kokturk. All Rights Reserved.


#ifdef ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/SwapChainD3D11.h>

#	include <Graphics/API/D3D/11/TextureD3D11.h>
#	include <d3d11.h>
#	include <Graphics/API/D3D/11/D3D11Helpers.h>
#	include <Graphics/API/D3D/11/FrameBufferD3D11.h>
#	include <Graphics/API/D3D/11/RenderPassD3D11.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	SwapChainD3D11::SwapChainD3D11(GraphicsD3D11& graphics, SystemHandle systemHandle, const Vec2i& size)
		: SwapChain(size, 2)
		, mGraphics(graphics)
		, mVSyncEnabled(0)
		, mHWND(nullptr)
	{
		mHWND = static_cast<HWND>(systemHandle.mWindow);
		ZE_ASSERT(mHWND, "Win32 window handle returned null!");

		const Vec2i& windowSize = GetSize();
		const MSAALevel msaaLevel = graphics.GetMultiSamplingLevel();

		DXGI_SWAP_CHAIN_DESC desc;
		desc.BufferDesc.Width = windowSize.X;
		desc.BufferDesc.Height = windowSize.Y;
		desc.BufferDesc.Format = D3D11Helpers::GetTextureFormat(mColorFormat);
		desc.BufferDesc.RefreshRate.Numerator = 0;	  // Get refresh rate of the window
		desc.BufferDesc.RefreshRate.Denominator = 0;  // Get refresh rate of the window
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		const UINT sampleCount = D3D11Helpers::GetMultiSampleLevel(msaaLevel);
		desc.SampleDesc.Count = sampleCount;
		desc.SampleDesc.Quality = 0;

		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	 // Buffer to be used as render target
		desc.BufferCount = GetFrameCount() - 1;				 // Back buffer count
		desc.OutputWindow = mHWND;

		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // TODO: Investigate behavior

		ZE_D3D_ASSERT_RESULT(mGraphics.GetFactoryD3D()->CreateSwapChain(mGraphics.GetDeviceD3D(), &desc, &mSwapChain));

		mSwapChainRenderPass = std::make_unique<RenderPassD3D11>(
			mGraphics,
			std::vector{ RenderPassAttachment{ mColorFormat, TextureLayout::Present } },
			RenderPassAttachment{ mDepthFormat, TextureLayout::DepthStencilAttachment },
			mGraphics.GetMultiSamplingLevel()
		);

		_createFrameBuffer();
	}

	SwapChainD3D11::~SwapChainD3D11() {}

	void SwapChainD3D11::Present()
	{
		ZE_D3D_ASSERT_RESULT(mSwapChain->Present(mVSyncEnabled, 0));
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
		_releaseFrameBuffer();
		_setSize(size);
		ZE_D3D_ASSERT_RESULT(mSwapChain->ResizeBuffers(0, size.X, size.Y, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
		_createFrameBuffer();
	}

	void SwapChainD3D11::_createFrameBuffer()
	{
		Gfx::ComPtr<ID3D11Texture2D> backBuffer;
		ZE_D3D_ASSERT_RESULT(GetSwapChainD3D()->GetBuffer(0, _uuidof(ID3D11Resource), &backBuffer));
		mColorTexture = std::make_unique<TextureD3D11>(std::move(backBuffer), GetSize(), TextureType::RenderTarget, mColorFormat, mSwapChainRenderPass->GetSampling());
		mDepthTexture = std::make_unique<TextureD3D11>(mGraphics, GetSize(), TextureType::Depth, mDepthFormat, nullptr, mSwapChainRenderPass->GetSampling());
		auto color = std::array<Texture*, 1>{ mColorTexture.get() };
		mFrameBuffer = std::make_unique<FrameBufferD3D11>(mGraphics, *mSwapChainRenderPass, GetSize(), color, mDepthTexture.get(), std::span<Texture*>{});
	}

	void SwapChainD3D11::_releaseFrameBuffer()
	{
		mFrameBuffer = nullptr;
		mColorTexture = nullptr;
		mDepthTexture = nullptr;
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