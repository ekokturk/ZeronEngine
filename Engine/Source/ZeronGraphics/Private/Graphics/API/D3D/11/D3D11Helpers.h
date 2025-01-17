// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/GraphicsTypes.h>


enum D3D11_BIND_FLAG;
enum D3D11_CULL_MODE;
enum DXGI_FORMAT;

namespace Zeron::Gfx
{
	class GraphicsD3D11;

	class D3D11Helpers {
	  public:
		static D3D11_BIND_FLAG GetBufferType(BufferType bufferType);
		static D3D11_CULL_MODE GetCullMode(FaceCullMode cullMode);
		static UINT GetMultiSampleLevel(MSAALevel level);
		static DXGI_FORMAT GetTextureFormat(TextureFormat textureFormat);
	};
}
#endif