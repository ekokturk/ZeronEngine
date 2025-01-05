// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/Texture.h>

struct ID3D11Texture2D;
struct ID3D11Device;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
enum D3D11_BIND_FLAG;

namespace Zeron::Gfx
{
	class GraphicsD3D11;

	class TextureD3D11 : public Texture {
	  public:
		TextureD3D11(GraphicsD3D11& graphics, const Vec2i& size, TextureType type, TextureFormat format, const void* data, MSAALevel sampling, bool shaderResource = false);
		TextureD3D11(Gfx::ComPtr<ID3D11Texture2D> texture, const Vec2i& size, TextureType type, TextureFormat format, MSAALevel sampling);

		ID3D11Texture2D* GetTextureD3D() const;
		ID3D11ShaderResourceView* GetOrCreateResourceViewD3D(ID3D11Device& device);
		ID3D11ShaderResourceView* GetResourceViewD3D() const;

		ComPtr<ID3D11RenderTargetView> CreateRenderTargetViewD3D(ID3D11Device& device) const;
		ComPtr<ID3D11DepthStencilView> CreateDepthStencilViewD3D(ID3D11Device& device) const;

	  private:
		D3D11_BIND_FLAG _getBindFlag() const;
		DXGI_FORMAT _getFormat(bool isDepth = false) const;

		Gfx::ComPtr<ID3D11Texture2D> mTexture;
		Gfx::ComPtr<ID3D11ShaderResourceView> mResourceView;
	};
}
#endif