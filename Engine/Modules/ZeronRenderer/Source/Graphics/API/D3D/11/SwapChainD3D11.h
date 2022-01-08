// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#if ZE_GRAPHICS_D3D
#include "Graphics/API/D3D/DXGI.h"
#include "Graphics/SwapChain.h"

struct IDXGISwapChain;
struct ID3D11DeviceContext;

namespace Zeron
{
	class GraphicsD3D11;
	class RenderTargetD3D11;
	class Window;

	class SwapChainD3D11 final : public SwapChain {
	public:
		SwapChainD3D11(Window& window, GraphicsD3D11& graphics);
		~SwapChainD3D11();

		virtual void SetViewport(const Vec2i& pos, const Vec2i& size) override;
		virtual void SwapBuffers() override;
		virtual void SetVSyncEnabled(bool isEnabled) override;
		virtual void SetRefreshRate(uint16_t rate) override;

		bool IsVSyncEnabled() const override;
		IDXGISwapChain* GetSwapChainD3D() const;
		RenderTarget* GetRenderTarget() const override;

	private:
		std::unique_ptr<RenderTargetD3D11> mRenderTarget;
		ZE::ComPtr<IDXGISwapChain> mSwapChain;
		UINT mVSyncEnabled;
		HWND mHWND;

		ID3D11DeviceContext* mDeviceContext;
	};
}
#endif
