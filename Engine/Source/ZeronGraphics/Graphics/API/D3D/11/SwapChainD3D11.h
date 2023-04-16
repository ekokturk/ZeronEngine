// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/GraphicsTypes.h>
#	include <Graphics/SwapChain.h>

struct IDXGISwapChain;

namespace Zeron
{
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
		std::unique_ptr<FrameBufferD3D11> mFrameBuffer;
		ZE::ComPtr<IDXGISwapChain> mSwapChain;
		UINT mVSyncEnabled;
		HWND mHWND;
	};
}
#endif