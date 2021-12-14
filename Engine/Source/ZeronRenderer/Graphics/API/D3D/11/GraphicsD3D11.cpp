// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "GraphicsD3D11.h"

#include "Graphics/API/D3D/DebugInfoD3D.h"

namespace Zeron
{
	bool GraphicsD3D11::Init()
	{
#if ZE_GRAPHICS_D3D
		auto& adapters = GetAdapters();
		if(adapters.empty()) {
			return false;
		}
		UINT creationFlags = 0;
	#if defined(_DEBUG)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		D3D_CALL(D3D11CreateDevice(
			adapters[0].GetAdapter(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&mDevice,
			nullptr,
			&mDeviceContext
		), false);
		return true;
#else
		return false;
#endif
	}

	bool GraphicsD3D11::Destroy()
	{
		return false;
	}
}
