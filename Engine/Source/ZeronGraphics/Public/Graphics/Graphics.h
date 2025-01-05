// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>
#include <Graphics/PipelineBinding.h>

namespace Zeron::Gfx
{
	struct PipelineConfig;
	struct RenderPassAttachment;
	class FrameBuffer;
	struct ShaderProgramConfig;
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

		virtual std::unique_ptr<Pipeline> CreatePipelineGraphics(ShaderProgram* shader, RenderPass* renderPass, PipelineConfig config) = 0;
		virtual std::unique_ptr<Pipeline> CreatePipelineCompute(ShaderProgram& shader) = 0;
		virtual std::unique_ptr<PipelineBinding> CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList) = 0;

		virtual std::unique_ptr<RenderPass> CreateRenderPass(
			std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling
		) = 0;
		virtual std::unique_ptr<FrameBuffer> CreateFrameBuffer(
			RenderPass& renderPass, const Vec2i& extent, const std::span<Texture*>& colorTextures, Texture* depthTexture = nullptr,
			const std::span<Texture*>& resolveTextures = {}
		) = 0;

		virtual std::unique_ptr<Buffer> CreateBuffer(BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage = BufferUsageType::Default) = 0;

		virtual std::unique_ptr<ShaderProgram> CreateShaderProgram(const ShaderProgramConfig& config, std::unordered_map<ShaderType, ByteBuffer> shaderData) = 0;
		virtual std::string GetCompiledShaderName(const std::string& shaderName, ShaderType type) const = 0;

		virtual std::unique_ptr<Texture> CreateTexture(
			const Vec2i& size, TextureFormat format = TextureFormat::RGBA_8U, const void* data = nullptr, TextureType type = TextureType::Sampling,
			MSAALevel sampling = MSAALevel::Disabled
		) = 0;

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

		template <typename T>
		std::unique_ptr<Buffer> CreateStorageBuffer(const std::vector<T>& data, bool useUnorderedAccess = false)
		{
			return CreateBuffer(
				BufferType::Storage, static_cast<uint32_t>(data.size()), sizeof(T), &data.front(), useUnorderedAccess ? BufferUsageType::Default : BufferUsageType::Dynamic
			);
		}

		std::unique_ptr<Buffer> CreateIndexBuffer(const std::vector<uint32_t>& data);
	};
}