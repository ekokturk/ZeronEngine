// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include "Graphics/GraphicsContext.h"
#include <wrl.h>
#include "Core/Types/Color.h"

struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

namespace Zeron {
	
	class GraphicsD3D11;

	class GraphicsContextD3D11 final : public GraphicsContext {
	public:
		GraphicsContextD3D11(Window* window, GraphicsD3D11& graphics);
		~GraphicsContextD3D11() override = default;

		virtual void ClearBuffer(Color color) override ;
		virtual void SetViewport(const Vec2i& pos, const Vec2i& size) override;
		virtual void SwapBuffers() override;
		virtual void SetVSyncEnabled(bool isEnabled) override;
		virtual void SetRefreshRate(uint16_t rate) override;

		[[nodiscard]] bool IsVSyncEnabled() const override;
		
		[[nodiscard]] ID3D11RenderTargetView* GetRenderTarget() const;
		
	private:

	private:
		GraphicsD3D11& mGraphics;
		UINT mVSyncEnabled;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTarget;
		HWND mHWND;
	};
}
#endif
