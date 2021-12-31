// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include "GraphicsAdapterD3D11.h"
#include "Graphics/API/D3D/DebugInfoD3D.h"

namespace Zeron
{
	GraphicsAdapterD3D11::GraphicsAdapterD3D11(ZE::ComPtr<IDXGIAdapter> adapter)
		: mAdapter(std::move(adapter))
	{
		ZeroMemory(&mDesc, sizeof(DXGI_ADAPTER_DESC));
		D3D_ASSERT_RESULT(mAdapter->GetDesc(&mDesc));
	}

	IDXGIAdapter* GraphicsAdapterD3D11::GetAdapterD3D() const
	{
		return mAdapter.Get();
	}
}
#endif
