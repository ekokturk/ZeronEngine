// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#if ZE_GRAPHICS_D3D
#include "Graphics/API/D3D/DXGI.h"
#include <d3d11.h>
#include "Graphics/Texture.h"

namespace Zeron
{
	class GraphicsD3D11;

	class TextureD3D11 : public Texture {
	public:
		TextureD3D11(GraphicsD3D11& graphics, const std::string& name);
		~TextureD3D11();

		ID3D11Texture2D* GetTextureD3D() const;
		ID3D11ShaderResourceView* GetResourceViewD3D() const;
	
	private:
		ZE::ComPtr<ID3D11Texture2D> mTexture;
		ZE::ComPtr<ID3D11ShaderResourceView> mResourceView;
		Vec2i mSize;
		int mChannel;
	};
}
#endif
