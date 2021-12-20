// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "BufferD3D11.h"
#include "ShaderD3D11.h"
#include "Graphics/Primitives.h"
#include "Graphics/API/D3D/GraphicsD3D.h"
#if ZE_GRAPHICS_D3D
#include "Graphics/API/D3D/11/BufferD3D11.h"
#include "Graphics/API/D3D/11/GraphicsContextD3D11.h"
#include <d3d11.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
#endif

namespace Zeron {

	class GraphicsContextD3D11;

	class GraphicsD3D11 final : public GraphicsD3D
	{
	public:
		virtual bool Init() override;
		virtual bool Destroy() override;
		virtual void RenderFrame() override;
		virtual void Draw() override;

		virtual void CreateGraphicsContext(Window* window) override;
		virtual void CreateVertexBuffer(const std::vector<Vertex>& data) override;

#if ZE_GRAPHICS_D3D
	public:
		void SetRasterizerState();
		void SetDepthStencilState();
		void SetSamplerState();
		bool InitializeShaders();
		
		[[nodiscard]] ID3D11Device* GetDevice() const;
		[[nodiscard]] ID3D11DeviceContext* GetDeviceContext() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;

		std::vector<std::shared_ptr<ShaderD3D11>> mShaders;
		std::vector<std::unique_ptr<GraphicsContextD3D11>> mGraphicsContexes;
		std::vector<std::unique_ptr<VertexBufferD3D11>> mVertexBuffers;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
#endif
	};
}
