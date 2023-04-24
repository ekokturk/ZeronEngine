// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/GraphicsTypes.h>
#	include <Graphics/Sampler.h>

struct ID3D11Buffer;
struct ID3D11SamplerState;
enum D3D11_TEXTURE_ADDRESS_MODE;

namespace Zeron::Gfx
{
	class GraphicsD3D11;

	class SamplerD3D11 : public Sampler {
	  public:
		SamplerD3D11(GraphicsD3D11& graphics, SamplerAddressMode addressMode, bool hasAnisotropicFilter);

		// D3D11 API
		ID3D11SamplerState* GetSamplerStateD3D() const;

	  protected:
		D3D11_TEXTURE_ADDRESS_MODE _getSamplerAddressMode(SamplerAddressMode addressMode);
		Gfx::ComPtr<ID3D11SamplerState> mSampler;
	};
}
#endif