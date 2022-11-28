// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsTypes.h>
#include <Graphics/API/D3D/11/GraphicsAdapterD3D11.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Zeron
{
	class GraphicsContext;
	class GraphicsAdapterD3D11;
	class GraphicsContextD3D11;

	class GraphicsD3D11 final : public Graphics {
	public:
		GraphicsD3D11();
		~GraphicsD3D11();
		
		bool Init() override;
		
		GraphicsType GetGraphicsType() const override;
		MSAALevel GetMultiSamplingLevel() const override;

		std::unique_ptr<GraphicsContext> CreateGraphicsContext() override;
		std::unique_ptr<CommandBuffer> CreateCommandBuffer(uint32_t count, bool isCompute) override;

		std::unique_ptr<Pipeline> CreatePipeline(ShaderProgram* shader) override;
		std::unique_ptr<Pipeline> CreatePipeline(ShaderProgram* shader, RenderPass* renderPass, MSAALevel samplingLevel, PrimitiveTopology topology, bool isSolidFill, FaceCullMode cullMode) override;
		std::unique_ptr<PipelineBinding> CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList) override;

		std::unique_ptr<Buffer> CreateBuffer(BufferType type, uint32_t size, uint32_t stride, const void* data, BufferUsageType usage = BufferUsageType::Default) override;

		std::unique_ptr<ShaderProgram> CreateShaderProgram(const std::string& shaderName, const std::string& shaderDirectory, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout) override;
		std::unique_ptr<ShaderProgram> CreateShaderProgram(const std::string& shaderName, const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout) override;

		std::unique_ptr<Texture> CreateTexture(TextureType type, const Color& data) override;
		std::unique_ptr<Texture> CreateTexture(TextureType type, const Color* data, uint32_t width, uint32_t height) override;
	
		std::unique_ptr<Sampler> CreateSampler(SamplerAddressMode addressMode = SamplerAddressMode::Repeat, bool hasAnisotropicFilter = true) override;

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
	};
}
#endif
