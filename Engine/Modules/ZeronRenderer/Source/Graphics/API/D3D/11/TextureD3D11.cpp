// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include "TextureD3D11.h"
#include "GraphicsD3D11.h"

#include <stb/stb_image.h>

#include "Graphics/API/D3D/DebugInfoD3D.h"


namespace Zeron
{
	TextureD3D11::TextureD3D11(GraphicsD3D11& graphics, TextureType type, const Color* data, uint32_t width, uint32_t height)
		: mSize{ static_cast<int>(width),static_cast<int>(height) }
	{
		mType = type;
		InitTexture_(graphics, data);
	}

	TextureD3D11::TextureD3D11(GraphicsD3D11& graphics, TextureType type, const std::string& filePath)
		: mSize { 1,1 }
	{
		mType = type;
		// TODO: This should be handled by asset importer class
		int channels = 0;
		if(stbi_uc* data = stbi_load(filePath.c_str(), &mSize.X, &mSize.Y, &channels, STBI_rgb_alpha))
		{
			InitTexture_(graphics, reinterpret_cast<Color*>(data));
			stbi_image_free(data);
		}
		else {
			// TODO: Have static colors for no texture and invalid texture
			const Color notFoundColor = Color::Pink;
			InitTexture_(graphics, &notFoundColor);
		}
	}

	TextureD3D11::~TextureD3D11()
	{
	}

	ID3D11ShaderResourceView* TextureD3D11::GetResourceViewD3D() const
	{
		return mResourceView.Get();
	}

	void TextureD3D11::InitTexture_(GraphicsD3D11& graphics, const Color* data)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = mSize.X;
		desc.Height = mSize.Y;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = mSize.X * sizeof(Color);

		D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateTexture2D(&desc, &bufferData, mTexture.GetAddressOf()));
		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, desc.Format);
		D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateShaderResourceView(mTexture.Get(), &srvDesc, mResourceView.GetAddressOf()));
	}

	ID3D11Texture2D* TextureD3D11::GetTextureD3D() const
	{
		return mTexture.Get();
	}
}
#endif