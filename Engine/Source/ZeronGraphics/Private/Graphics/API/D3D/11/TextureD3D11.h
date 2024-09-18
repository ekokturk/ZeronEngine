// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/Texture.h>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace Zeron::Gfx
{
	class GraphicsD3D11;

	class TextureD3D11 : public Texture {
	  public:
		TextureD3D11(GraphicsD3D11& graphics, TextureType type, const Color* data, uint32_t width, uint32_t height);

		ID3D11Texture2D* GetTextureD3D() const;
		ID3D11ShaderResourceView* GetResourceViewD3D() const;

	  private:
		void InitTexture_(GraphicsD3D11& graphics, const Color* data);

		Gfx::ComPtr<ID3D11Texture2D> mTexture;
		Gfx::ComPtr<ID3D11ShaderResourceView> mResourceView;
		Vec2i mSize;
	};
}
#endif