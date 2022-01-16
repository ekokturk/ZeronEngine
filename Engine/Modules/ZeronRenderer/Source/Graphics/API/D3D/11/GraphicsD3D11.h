// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include "BufferD3D11.h"
#include "ShaderD3D11.h"
#include "Graphics/Primitives.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicsTypes.h"
#if ZE_GRAPHICS_D3D
#include "GraphicsAdapterD3D11.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11SamplerState;
#endif

namespace Zeron
{
	class Texture;
	class SwapChain;
	class GraphicsContext;
	class GraphicsAdapterD3D11;
	class GraphicsContextD3D11;

	class GraphicsD3D11 final : public Graphics {
	public:
		GraphicsD3D11();
		~GraphicsD3D11();
		
		virtual bool Init() override;
		
		GraphicsType GetGraphicsType() const override;
		std::shared_ptr<GraphicsContext> GetImmediateContext() const override;

		// Graphics
		virtual std::shared_ptr<GraphicsContext> CreateGraphicsContext() override;
		virtual std::shared_ptr<SwapChain> CreateSwapChain(Window& window) override;

		// Shader
		std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& shaderName,
			const std::string& shaderDirectory, const VertexLayout& layout) override;
		std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& shaderName,
			const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader,
			const VertexLayout& layout) override;

		// Texture
		virtual std::shared_ptr<Texture> CreateTexture(TextureType type, const Color& data) override;
		virtual std::shared_ptr<Texture> CreateTexture(TextureType type, const Color* data, uint32_t width, uint32_t height) override;
		virtual std::shared_ptr<Texture> CreateTexture(TextureType type, const std::string& path) override;
	
	protected:
		std::shared_ptr<Buffer> CreateBuffer(BufferType type, const void* data, uint32_t size, uint32_t stride) override;

#if ZE_GRAPHICS_D3D
	public:
		const std::vector<GraphicsAdapterD3D11>& GetAdapters();
		IDXGIFactory* GetFactoryD3D() const;
		IDXGIAdapter* GetPrimaryAdapterD3D();
		ID3D11Device* GetDeviceD3D() const;
		ID3D11DeviceContext* GetDeviceContextD3D() const;

		
	private:
		ZE::ComPtr<IDXGIFactory> mFactory;
		std::vector<GraphicsAdapterD3D11> mGraphicsAdapters;
		ZE::ComPtr<ID3D11Device> mDevice;
		ZE::ComPtr<ID3D11DeviceContext> mDeviceContext;
		
		std::shared_ptr<GraphicsContextD3D11> mImmediateContext;
#endif
	};
}
