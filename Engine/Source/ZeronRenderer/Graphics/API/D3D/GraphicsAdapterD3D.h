// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <wrl/client.h>
#include "dxgi.h"
#endif

struct IDXGIAdapter;

namespace Zeron {

	class GraphicsAdapterD3D {
	public:
#if ZE_GRAPHICS_D3D
		GraphicsAdapterD3D(Microsoft::WRL::ComPtr<IDXGIAdapter> adapter);
		
		IDXGIAdapter* GetAdapter() const;
		[[nodiscard]] const DXGI_ADAPTER_DESC& GetDescription() const { return mDesc; }
	
	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter> mAdapter;
		DXGI_ADAPTER_DESC mDesc;
#endif
	};

}
