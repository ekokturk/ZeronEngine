// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/GraphicsAdapterD3D11.h>
#	include <Graphics/Graphics.h>
#	include <Graphics/GraphicsTypes.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Zeron::Gfx
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

		std::unique_ptr<Pipeline> CreatePipelineGraphics(ShaderProgram* shader, RenderPass* renderPass, PipelineConfig config) override;
		std::unique_ptr<Pipeline> CreatePipelineCompute(ShaderProgram& shader) override;
		std::unique_ptr<PipelineBinding> CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList) override;

		std::unique_ptr<RenderPass> CreateRenderPass(
			std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling
		) override;
		std::unique_ptr<FrameBuffer> CreateFrameBuffer(
			RenderPass& renderPass, const Vec2i& extent, const std::span<Texture*>& colorTextures, Texture* depthTexture, const std::span<Texture*>& resolveTextures
		) override;

		std::unique_ptr<Buffer> CreateBuffer(BufferType type, uint32_t size, uint32_t stride, const void* data, BufferUsageType usage) override;

		std::unique_ptr<ShaderProgram> CreateShaderProgram(const ShaderProgramConfig& config, std::unordered_map<ShaderType, ByteBuffer> shaderData) override;
		std::string GetCompiledShaderName(const std::string& shaderName, ShaderType type) const override;

		std::unique_ptr<Texture> CreateTexture(const Vec2i& size, TextureFormat format, const void* data, TextureType type, MSAALevel sampling) override;

		std::unique_ptr<Sampler> CreateSampler(SamplerAddressMode addressMode = SamplerAddressMode::Repeat, bool hasAnisotropicFilter = true) override;

	  public:
		const std::vector<GraphicsAdapterD3D11>& GetAdapters();
		IDXGIFactory* GetFactoryD3D() const;
		IDXGIAdapter* GetPrimaryAdapterD3D();
		ID3D11Device* GetDeviceD3D() const;
		ID3D11DeviceContext* GetDeviceContextD3D() const;

	  private:
		MSAALevel _getMaxMultiSampleLevel() const;

		Gfx::ComPtr<IDXGIFactory> mFactory;
		std::vector<GraphicsAdapterD3D11> mGraphicsAdapters;
		Gfx::ComPtr<ID3D11Device> mDevice;
		Gfx::ComPtr<ID3D11DeviceContext> mDeviceContext;

		std::shared_ptr<GraphicsContextD3D11> mImmediateContext;

		MSAALevel mMaxSupportedSampling;
	};
}
#endif