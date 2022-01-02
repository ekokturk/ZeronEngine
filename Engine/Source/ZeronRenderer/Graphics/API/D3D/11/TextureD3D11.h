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
		TextureD3D11(GraphicsD3D11& graphics, TextureType type, const Color* data, uint32_t width, uint32_t height);
		TextureD3D11(GraphicsD3D11& graphics, TextureType type, const std::string& filePath);
		
		~TextureD3D11();

		[[nodiscard]] ID3D11Texture2D* GetTextureD3D() const;
		[[nodiscard]] ID3D11ShaderResourceView* GetResourceViewD3D() const;
	
	private:
		void InitTexture_(GraphicsD3D11& graphics, const Color* data);

		ZE::ComPtr<ID3D11Texture2D> mTexture;
		ZE::ComPtr<ID3D11ShaderResourceView> mResourceView;
		Vec2i mSize;
	};
}
#endif
