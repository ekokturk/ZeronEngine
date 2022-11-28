// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/GraphicsTypes.h>
#include <Graphics/API/D3D/DXGI.h>


enum D3D11_BIND_FLAG;
enum D3D11_CULL_MODE;

namespace Zeron
{
	class GraphicsD3D11;

	class D3D11Helpers {
	public:
		static D3D11_BIND_FLAG GetBufferType(BufferType bufferType);
		static D3D11_CULL_MODE GetCullMode(FaceCullMode cullMode);
		static UINT GetMultiSampleLevel(MSAALevel level);
	};
}
#endif
