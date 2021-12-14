// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Graphics/API/D3D/GraphicsD3D.h"
#if ZE_GRAPHICS_D3D
#include <d3d11.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
#endif

namespace Zeron {
	
	class GraphicsD3D11 final : public GraphicsD3D
	{
	public:
		virtual bool Init() override;
		virtual bool Destroy() override;
		
#if ZE_GRAPHICS_D3D
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
#endif
	};
}
