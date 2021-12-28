// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GraphicsAdapterD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"

namespace Zeron {
#if ZE_GRAPHICS_D3D
	
	GraphicsAdapterD3D11::GraphicsAdapterD3D11(ZE::ComPtr<IDXGIAdapter> adapter)
		: mAdapter(std::move(adapter))
		, mDesc(DXGI_ADAPTER_DESC())
	{
		D3D_ASSERT_RESULT(mAdapter->GetDesc(&mDesc));
	}

	IDXGIAdapter* GraphicsAdapterD3D11::GetAdapterD3D() const
	{
		return mAdapter.Get();
	}

#endif
}
