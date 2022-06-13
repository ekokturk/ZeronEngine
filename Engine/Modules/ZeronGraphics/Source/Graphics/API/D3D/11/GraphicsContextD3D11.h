// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/DXGI.h>
#include <Graphics/GraphicsContext.h>
#include <d3d11.h>

struct ID3D11DepthStencilState;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

namespace Zeron
{
	class Buffer;
	class GraphicsD3D11;
	class RenderTargetD3D11;
	class Shader;
	class Texture;

	class GraphicsContextD3D11 final : public GraphicsContext {
	public:
		GraphicsContextD3D11(GraphicsD3D11& graphics);
		~GraphicsContextD3D11();

		virtual void SetPrimitiveTopology(PrimitiveTopology topology) override;
		virtual void SetVertexBuffer(Buffer& vb, uint32_t slot) override;
		virtual void SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot = 0) override;
		virtual void SetIndexBuffer(Buffer& ib) override;
		virtual void SetConstantBuffer(Buffer& cb, ShaderType type, uint32_t slot) override;
		virtual void SetShader(Shader* shader) override;
		virtual void SetShaderProgram(ShaderProgram* program) override;
		virtual void SetTexture(Texture* texture, uint8_t slot) override;

		virtual void UpdateBuffer(Buffer& buff, void* data, uint32_t sizeBytes) override;
		
		virtual void SetFillMode(bool isSolid) override;
		
		virtual void SetRenderTarget(RenderTarget* target) override;

		virtual void Draw(uint32_t vertexCount, uint32_t vertexStart) override;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t indexStart) override;
		virtual void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart, uint32_t instanceStart)  override;
		virtual void DrawInstancedIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart, uint32_t baseVertexLocation, uint32_t instanceStart)  override;

		virtual void Clear(Color color) override;

		virtual void SetViewport(const Vec2i& size, const Vec2i& position) override;
		virtual void ResizeSwapChain(SwapChain& swapChain, const Vec2i& size) override;

	private:
		// Pipeline States
		void CreateRasterizerState(D3D11_FILL_MODE fillMode);
		void CreateDepthStencilState();
		void CreateSamplerState();
		void CreateBlendState();

	private:
		ZE::ComPtr<ID3D11RasterizerState> mRasterizerState;
		ZE::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
		ZE::ComPtr<ID3D11SamplerState> mSamplerState;
		ZE::ComPtr<ID3D11BlendState> mBlendState;
	
	private:
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		ZE::ComPtr<ID3D11DeviceContext> mDefferedContext;

		PrimitiveTopology mPrimitiveTopology;
		RenderTargetD3D11* mRenderTarget;
	};
}
#endif
