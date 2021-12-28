// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#if ZE_GRAPHICS_D3D
#include "Graphics/API/D3D/DXGI.h"
#include "Graphics/GraphicsContext.h"
#include <d3d11.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11SamplerState;

namespace Zeron
{
	class Texture;
	class Buffer;
	class Shader;
	class RenderTargetD3D11;
	class GraphicsD3D11;

	class GraphicsContextD3D11 final : public GraphicsContext {
	public:
		GraphicsContextD3D11(GraphicsD3D11& graphics);
		~GraphicsContextD3D11();

		virtual void SetPrimitiveTopology(PrimitiveTopology topology) override;
		virtual void SetVertexBuffer(Buffer& vb) override;
		virtual void SetIndexBuffer(Buffer& ib) override;
		virtual void SetConstantBuffer(Buffer& cb) override;
		virtual void SetShader(Shader* shader) override;
		virtual void SetTexture(Texture* texture) override;

		virtual void UpdateBuffer(Buffer& buff, void* data, uint32_t size) override;
		
		virtual void SetFillMode(bool isSolid) override;
		
		virtual void SetRenderTarget(RenderTarget* target) override;

		virtual void DrawIndexed(uint32_t indexCount, uint32_t indexStart) override;
		virtual void Clear(Color color) override;

	private:
		// Pipeline States
		void CreateRasterizerState(D3D11_FILL_MODE fillMode);
		void CreateDepthStencilState();
		void CreateSamplerState();

		ZE::ComPtr<ID3D11RasterizerState> mRasterizerState;
		ZE::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
		ZE::ComPtr<ID3D11SamplerState> mSamplerState;
	
	private:
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		ZE::ComPtr<ID3D11DeviceContext> mDefferedContext;

		PrimitiveTopology mPrimitiveTopology;
		RenderTargetD3D11* mRenderTarget;
	};
}
#endif
