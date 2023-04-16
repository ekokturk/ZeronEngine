// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/DXGI.h>
#	include <Graphics/GraphicsTypes.h>
#	include <Graphics/Pipeline.h>

struct ID3D11DepthStencilState;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;
struct ID3D11BlendState;
enum D3D_PRIMITIVE_TOPOLOGY;

namespace Zeron
{
	class RenderPass;
	class ShaderProgramD3D11;
	class GraphicsD3D11;
	class SwapChainD3D11;

	class PipelineD3D11 : public Pipeline {
	  public:
		PipelineD3D11(
			GraphicsD3D11& graphics, ShaderProgramD3D11* shader, RenderPass* renderPass, MSAALevel samplingLevel, PrimitiveTopology topology = PrimitiveTopology::TriangleList,
			bool isSolidFill = true, FaceCullMode cullMode = FaceCullMode::Back
		);

		// D3D11 API
		void Apply(ID3D11DeviceContext* deviceContext);
		ShaderProgramD3D11* GetShaderProgramD3D() const;

	  private:
		ShaderProgramD3D11* mShader;

		D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology;

		ZE::ComPtr<ID3D11RasterizerState> mRasterizerState;
		ZE::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
		ZE::ComPtr<ID3D11BlendState> mBlendState;
	};
}
#endif