// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GraphicsAdapterD3D.h"
#include "DebugInfoD3D.h"

namespace Zeron {
#if ZE_GRAPHICS_D3D
	
	GraphicsAdapterD3D::GraphicsAdapterD3D(Microsoft::WRL::ComPtr<IDXGIAdapter> adapter)
		: mAdapter(std::move(adapter))
		, mDesc(DXGI_ADAPTER_DESC())
	{
		D3D_CALL(mAdapter->GetDesc(&mDesc));
	}

	IDXGIAdapter* GraphicsAdapterD3D::GetAdapter() const
	{
		return mAdapter.Get();
	}

#endif
}
