// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/GraphicsAdapter.h>
#include <Graphics/API/D3D/DXGI.h>

struct IDXGIAdapter;

namespace Zeron
{
	class GraphicsAdapterD3D11 final : public GraphicsAdapter {
	public:
		GraphicsAdapterD3D11(ZE::ComPtr<IDXGIAdapter> adapter);
		
		const char* GetName() const override;
		bool HasSwapChainSupport() const override;
		bool IsDiscreteAdapter() const override;

		IDXGIAdapter* GetAdapterD3D() const;
		const DXGI_ADAPTER_DESC& GetDescriptionD3D() const { return mDesc; }

	private:
		ZE::ComPtr<IDXGIAdapter> mAdapter;
		DXGI_ADAPTER_DESC mDesc;
		std::string mName;
	};
}
#endif
