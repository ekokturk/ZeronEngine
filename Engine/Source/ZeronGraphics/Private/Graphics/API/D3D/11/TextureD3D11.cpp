// Copyright (C) Eser Kokturk. All Rights Reserved.


#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/TextureD3D11.h>

#	include <d3d11.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>
#	include <Graphics/API/D3D/11/D3D11Helpers.h>

namespace Zeron::Gfx
{
	TextureD3D11::TextureD3D11(GraphicsD3D11& graphics, const Vec2i& size, TextureType type, TextureFormat format, const void* data, MSAALevel sampling, bool shaderResource)
		: Texture(size, type, format, sampling)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = GetExtent().X;
		desc.Height = GetExtent().Y;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = D3D11Helpers::GetTextureFormat(GetFormat());
		desc.SampleDesc.Count = D3D11Helpers::GetMultiSampleLevel(GetSampling());
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		desc.BindFlags = _getBindFlag();
		if (shaderResource) {
			desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		}
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		if (data) {
			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
			bufferData.pSysMem = data;
			bufferData.SysMemPitch = data ? GetExtent().X * GetStride() : 0;
			ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateTexture2D(&desc, &bufferData, mTexture.GetAddressOf()));
		}
		else {
			ZE_D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateTexture2D(&desc, nullptr, mTexture.GetAddressOf()));
		}
	}

	TextureD3D11::TextureD3D11(Gfx::ComPtr<ID3D11Texture2D> texture, const Vec2i& size, TextureType type, TextureFormat format, MSAALevel sampling)
		: Texture(size, type, format, sampling)
		, mTexture(std::move(texture))
	{}

	ID3D11ShaderResourceView* TextureD3D11::GetResourceViewD3D() const
	{
		ZE_ASSERT(GetType() == TextureType::Sampling, "Texture type needs to be Sampling to use ID3D11ShaderResourceView");
		return mResourceView.Get();
	}

	ID3D11ShaderResourceView* TextureD3D11::GetOrCreateResourceViewD3D(ID3D11Device& device)
	{
		if (!mResourceView) {
			const D3D11_SRV_DIMENSION dimension = GetSampling() == MSAALevel::Disabled ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DMS;
			const CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, _getFormat());
			ZE_D3D_ASSERT(device.CreateShaderResourceView(mTexture.Get(), &srvDesc, mResourceView.GetAddressOf()));
		}
		return mResourceView.Get();
	}

	ComPtr<ID3D11RenderTargetView> TextureD3D11::CreateRenderTargetViewD3D(ID3D11Device& device) const
	{
		Gfx::ComPtr<ID3D11RenderTargetView> view;
		const D3D11_RTV_DIMENSION dimension = GetSampling() == MSAALevel::Disabled ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DMS;
		const CD3D11_RENDER_TARGET_VIEW_DESC rtvDesc(dimension);
		ZE_D3D_ASSERT(device.CreateRenderTargetView(mTexture.Get(), &rtvDesc, view.GetAddressOf()));
		return view;
	}

	ComPtr<ID3D11DepthStencilView> TextureD3D11::CreateDepthStencilViewD3D(ID3D11Device& device) const
	{
		Gfx::ComPtr<ID3D11DepthStencilView> view;
		const D3D11_DSV_DIMENSION dimension = GetSampling() == MSAALevel::Disabled ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DMS;
		const CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(dimension, _getFormat(true));
		ZE_D3D_ASSERT(device.CreateDepthStencilView(mTexture.Get(), &dsvDesc, view.GetAddressOf()));
		return view;
	}

	D3D11_BIND_FLAG TextureD3D11::_getBindFlag() const
	{
		switch (GetType()) {
			case TextureType::Sampling: return D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
			case TextureType::RenderTarget: return D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
			case TextureType::Depth: return D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
			default: ZE_FAIL("Texture type is not supported.");
		}
		return static_cast<D3D11_BIND_FLAG>(0);
	}

	DXGI_FORMAT TextureD3D11::_getFormat(bool isDepth) const
	{
		DXGI_FORMAT format = D3D11Helpers::GetTextureFormat(GetFormat());
		switch (format) {
			case DXGI_FORMAT_R16_TYPELESS: return isDepth ? DXGI_FORMAT_D16_UNORM : DXGI_FORMAT_R16_UNORM;
			case DXGI_FORMAT_R32_TYPELESS: return isDepth ? DXGI_FORMAT_D32_FLOAT : DXGI_FORMAT_R32_FLOAT;
			case DXGI_FORMAT_R24G8_TYPELESS: return isDepth ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}
		return format;
	}

	ID3D11Texture2D* TextureD3D11::GetTextureD3D() const
	{
		return mTexture.Get();
	}


}
#endif