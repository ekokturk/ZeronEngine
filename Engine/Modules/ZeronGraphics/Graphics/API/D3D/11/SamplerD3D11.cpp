// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/11/SamplerD3D11.h>

#include <d3d11.h>
#include <Graphics/API/D3D/DebugInfoD3D.h>
#include <Graphics/API/D3D/11/GraphicsD3D11.h>

namespace Zeron
{
	SamplerD3D11::SamplerD3D11(GraphicsD3D11& graphics, SamplerAddressMode addressMode, bool hasAnisotropicFilter)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
		samplerDesc.Filter = hasAnisotropicFilter ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		samplerDesc.AddressU = _getSamplerAddressMode(addressMode);
		samplerDesc.AddressV = _getSamplerAddressMode(addressMode);
		samplerDesc.AddressW = _getSamplerAddressMode(addressMode);
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		if(hasAnisotropicFilter) {
			samplerDesc.MaxAnisotropy = 8;
		}
		D3D_ASSERT_RESULT(graphics.GetDeviceD3D()->CreateSamplerState(&samplerDesc, mSampler.GetAddressOf()));
	}

	ID3D11SamplerState* SamplerD3D11::GetSamplerStateD3D() const
	{
		return mSampler.Get();
	}

	D3D11_TEXTURE_ADDRESS_MODE SamplerD3D11::_getSamplerAddressMode(SamplerAddressMode addressMode)
	{
		switch (addressMode) {
			case SamplerAddressMode::Repeat: return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
			case SamplerAddressMode::Clamp: return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
			default: ZE_FAIL("D3D11 texture address mode is not supported!");
		}
		return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	}
}
#endif
