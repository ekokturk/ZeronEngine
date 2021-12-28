// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include "TextureD3D11.h"
#include "GraphicsD3D11.h"

#include <stb/stb_image.h>

#include "Graphics/API/D3D/DebugInfoD3D.h"


namespace Zeron
{
	TextureD3D11::TextureD3D11(GraphicsD3D11& graphics, const std::string& name)
		: mChannel(0)
	{
		// TODO: This can be moved to a load method
		const std::string path = Texture::RESOURCE_PATH + name;
		if(stbi_uc* textData = stbi_load(path.c_str(), &mSize.X, &mSize.Y, &mChannel, STBI_rgb_alpha))
		{
			D3D11_TEXTURE2D_DESC textDesc;
			textDesc.Width = mSize.X;
			textDesc.Height = mSize.Y;
			textDesc.MipLevels = 1;
			textDesc.ArraySize = 1;
			textDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			textDesc.SampleDesc.Count = 1;
			textDesc.SampleDesc.Quality = 0;
			textDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
			textDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
			textDesc.CPUAccessFlags = 0;
			textDesc.MiscFlags = 0;
			
			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
			bufferData.pSysMem = textData;
			bufferData.SysMemPitch = mSize.X * STBI_rgb_alpha;

			D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateTexture2D(&textDesc, &bufferData, mTexture.GetAddressOf()));
			D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateShaderResourceView(mTexture.Get(), nullptr, &mResourceView));
			
			stbi_image_free(textData);
		}
	}

	TextureD3D11::~TextureD3D11()
	{
	}

	ID3D11ShaderResourceView* TextureD3D11::GetResourceViewD3D() const
	{
		return mResourceView.Get();
	}

	ID3D11Texture2D* TextureD3D11::GetTextureD3D() const
	{
		return mTexture.Get();
	}
}
#endif