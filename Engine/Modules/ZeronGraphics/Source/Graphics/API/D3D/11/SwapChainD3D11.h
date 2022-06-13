// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/DXGI.h>
#include <Graphics/SwapChain.h>

struct IDXGISwapChain;

namespace Zeron
{
	class GraphicsD3D11;
	class RenderTargetD3D11;
	class Window;

	class SwapChainD3D11 final : public SwapChain {
	public:
		SwapChainD3D11(GraphicsD3D11& graphics, void* windowHandle, const Vec2i& size);
		~SwapChainD3D11();

		virtual void SwapBuffers() override;
		virtual void SetVSyncEnabled(bool isEnabled) override;
		virtual void SetRefreshRate(uint16_t rate) override;

		bool IsVSyncEnabled() const override;
		RenderTarget* GetRenderTarget() const override;

		// D3D11
		IDXGISwapChain* GetSwapChainD3D() const;
		void Resize(const Vec2i& size);

	private:
		std::unique_ptr<RenderTargetD3D11> mRenderTarget;
		ZE::ComPtr<IDXGISwapChain> mSwapChain;
		UINT mVSyncEnabled;
		HWND mHWND;
	};
}
#endif
