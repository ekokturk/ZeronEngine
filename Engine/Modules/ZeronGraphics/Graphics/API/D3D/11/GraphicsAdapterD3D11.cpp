// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/API/D3D/11/GraphicsAdapterD3D11.h>

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/DebugInfoD3D.h>

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
