// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/DXGI.h>

struct IDXGIAdapter;

namespace Zeron
{
	class GraphicsAdapterD3D11 {
	public:
		GraphicsAdapterD3D11(ZE::ComPtr<IDXGIAdapter> adapter);
		
		IDXGIAdapter* GetAdapterD3D() const;
		const DXGI_ADAPTER_DESC& GetDescription() const { return mDesc; }
	
	private:
		ZE::ComPtr<IDXGIAdapter> mAdapter;
		DXGI_ADAPTER_DESC mDesc;
	};
}
#endif
