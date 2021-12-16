// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GraphicsD3D.h"
#if ZE_GRAPHICS_D3D
#include "DebugInfoD3D.h"
#include "dxgi.h"
#endif

namespace Zeron {

	std::vector<GraphicsAdapterD3D> GraphicsD3D::mGraphicsAdapters;

	GraphicsD3D::~GraphicsD3D()
	{
		mGraphicsAdapters.clear();
	}

	std::vector<GraphicsAdapterD3D>& GraphicsD3D::GetAdapters()
	{
#if ZE_GRAPHICS_D3D
		if(mGraphicsAdapters.empty()) {
			Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
			D3D_ASSERT_RESULT(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())), mGraphicsAdapters);
			Microsoft::WRL::ComPtr<IDXGIAdapter>  dxgiAdapter;
			UINT index = 0;
			while (SUCCEEDED(dxgiFactory->EnumAdapters(index, &dxgiAdapter))) {
				mGraphicsAdapters.emplace_back(GraphicsAdapterD3D(dxgiAdapter));
				index++;
			}
		}
#endif

		return mGraphicsAdapters;
	}

	IDXGIFactory* GraphicsD3D::GetFactoryDXGI() const
	{
		ZE_ASSERT(mFactory, "DXGI factory is null!");
		return mFactory.Get();
	}

	IDXGIAdapter* GraphicsD3D::GetPrimaryAdapterDXGI() const
	{
		auto& adapterList = GetAdapters();
		ZE_ASSERT(!adapterList.empty(), "No graphics adapters found!");
		return adapterList[0].GetAdapter();
	}
}
