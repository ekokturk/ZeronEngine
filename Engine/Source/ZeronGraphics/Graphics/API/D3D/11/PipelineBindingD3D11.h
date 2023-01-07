// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/PipelineBinding.h>

#include <Graphics/API/D3D/DXGI.h>

struct ID3D11Buffer;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

namespace Zeron
{
	class PipelineD3D11;
	class GraphicsD3D11;

	class PipelineBindingD3D11 : public PipelineBinding {
		struct BindingD3D {
			uint32_t mSlot;
			ShaderType mShaderStage;
			std::vector<ID3D11Buffer*> mBuffer;
			std::vector<ID3D11SamplerState*> mSampler;
			std::vector<ID3D11ShaderResourceView*> mTexture;
		};

	public:
		PipelineBindingD3D11(GraphicsD3D11& graphics, PipelineD3D11& pipeline, const std::vector<BindingHandle>& bindings);

		// D3D11 API
		void Apply(ID3D11DeviceContext* deviceContext);

	private:
		std::vector<BindingD3D> mConstantBuffers;
		std::vector<BindingD3D> mTextures;
		std::vector<BindingD3D> mSamplers;
	};
}
#endif
