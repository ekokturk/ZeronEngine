// Copyright (C) Eser Kokturk. All Rights Reserved.


#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/GraphicsAdapterD3D11.h>

#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	GraphicsAdapterD3D11::GraphicsAdapterD3D11(Gfx::ComPtr<IDXGIAdapter> adapter)
		: mAdapter(std::move(adapter))
	{
		ZeroMemory(&mDesc, sizeof(DXGI_ADAPTER_DESC));
		ZE_D3D_ASSERT_RESULT(mAdapter->GetDesc(&mDesc));

		mName.reserve(256);
		std::wcstombs(mName.data(), mDesc.Description, 256);
	}

	const char* GraphicsAdapterD3D11::GetName() const
	{
		return mName.c_str();
	}

	bool GraphicsAdapterD3D11::HasSwapChainSupport() const
	{
		return true;
	}

	bool GraphicsAdapterD3D11::IsDiscreteAdapter() const
	{
		return true;
	}

	IDXGIAdapter* GraphicsAdapterD3D11::GetAdapterD3D() const
	{
		return mAdapter.Get();
	}
}
#endif