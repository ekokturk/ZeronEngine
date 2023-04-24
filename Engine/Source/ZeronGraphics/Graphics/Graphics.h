// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>
#include <Graphics/PipelineBinding.h>

namespace Zeron::Gfx
{
	class CommandBuffer;
	class Sampler;
	class RenderPass;
	class Pipeline;
	class ResourceLayout;
	class Buffer;
	class CommandQueue;
	class SwapChain;
	class GraphicsContext;
	class ShaderProgram;
	class Shader;
	class VertexLayout;
	class Window;
	class Texture;

	class Graphics {
	  public:
		static std::unique_ptr<Graphics> CreateGraphics(GraphicsType api);

		virtual ~Graphics() = default;

		virtual bool Init() = 0;
		virtual GraphicsType GetGraphicsType() const = 0;
		virtual MSAALevel GetMultiSamplingLevel() const = 0;

		virtual std::unique_ptr<GraphicsContext> CreateGraphicsContext() = 0;
		virtual std::unique_ptr<CommandBuffer> CreateCommandBuffer(uint32_t count = 1, bool isCompute = false) = 0;

		virtual std::unique_ptr<Pipeline> CreatePipeline(ShaderProgram* shader) = 0;
		virtual std::unique_ptr<Pipeline> CreatePipeline(
			ShaderProgram* shader, RenderPass* renderPass, MSAALevel samplingLevel, PrimitiveTopology topology = PrimitiveTopology::TriangleList, bool isSolidFill = true,
			FaceCullMode cullMode = FaceCullMode::Back
		) = 0;
		virtual std::unique_ptr<PipelineBinding> CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList) = 0;

		virtual std::unique_ptr<Buffer> CreateBuffer(BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage = BufferUsageType::Default) = 0;

		virtual std::unique_ptr<ShaderProgram> CreateShaderProgram(
			const std::string& shaderName, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout, const ByteBuffer& vertexShader = {},
			const ByteBuffer& fragmentShader = {}, const ByteBuffer& computeShader = {}
		) = 0;
		virtual std::unique_ptr<ShaderProgram> CreateShaderProgram(
			const std::string& shaderName, const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader, const VertexLayout& vertexLayout,
			const ResourceLayout& resourceLayout
		) = 0;
		virtual std::string GetCompiledShaderName(const std::string& shaderName, ShaderType type) const = 0;

		virtual std::unique_ptr<Texture> CreateTexture(TextureType type, const Color& data) = 0;
		virtual std::unique_ptr<Texture> CreateTexture(TextureType type, const Color* data, uint32_t width, uint32_t height) = 0;

		virtual std::unique_ptr<Sampler> CreateSampler(SamplerAddressMode addressMode = SamplerAddressMode::Repeat, bool hasAnisotropicFilter = true) = 0;

		template <typename T>
		std::unique_ptr<Buffer> CreateVertexBuffer(const std::vector<T>& data)
		{
			return CreateBuffer(BufferType::Vertex, static_cast<uint32_t>(data.size()), sizeof(T), &data.front());
		}

		template <typename T>
		std::unique_ptr<Buffer> CreateUniformBuffer(const T& data)
		{
			return CreateBuffer(BufferType::Uniform, 1, sizeof(T), &data, BufferUsageType::Dynamic);
		}

		std::unique_ptr<Buffer> CreateIndexBuffer(const std::vector<uint32_t>& data);
	};
}