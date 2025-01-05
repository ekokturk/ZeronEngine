// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class Texture;
	class Buffer;
	class Pipeline;
	class PipelineBinding;
	class RenderPass;
	class FrameBuffer;

	class CommandBuffer {
	  public:
		virtual ~CommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void BeginRenderPass(FrameBuffer* frameBuffer) = 0;
		virtual void EndRenderPass() = 0;
		virtual void End() = 0;

		virtual void Clear(Color color) = 0;
		virtual void SetScissor(const Vec2i& extent, const Vec2i& offset = Vec2i()) = 0;
		virtual void SetViewport(const Vec2i& size, const Vec2i& position = Vec2i()) = 0;
		virtual void SetPipeline(Pipeline& pipeline) = 0;
		virtual void SetPipelineBinding(PipelineBinding& binding, uint32_t index = 0) = 0;

		virtual void SetVertexBuffer(Buffer& vb, uint32_t slot = 0) = 0;
		virtual void SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot = 0) = 0;
		virtual void SetIndexBuffer(Buffer& ib) = 0;

		virtual void CopyBuffer(Buffer& source, Buffer& destination) = 0;
		virtual void UpdateBuffer(Buffer& buff, const void* data, uint32_t sizeBytes, uint32_t offset = 0, BufferUpdateRule updateRule = BufferUpdateRule::UnmapMemory) = 0;
		template <typename T>
		void UpdateBuffer(Buffer& buff, const T* data)
		{
			UpdateBuffer(buff, data, sizeof(T));
		}
		virtual void SetPushConstant(const void* data, uint32_t stride, ShaderType shaderType) = 0;
		template <typename T>
		void SetPushConstant(const T* data, ShaderType shaderType)
		{
			SetPushConstant(data, sizeof(T), shaderType);
		}

		virtual void Draw(uint32_t vertexCount, uint32_t vertexStart = 0) = 0;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t indexStart = 0, uint32_t vertexStart = 0) = 0;
		virtual void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart = 0, uint32_t instanceStart = 0) = 0;
		virtual void DrawInstancedIndexed(
			uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart = 0, uint32_t baseVertexLocation = 0, uint32_t instanceStart = 0
		) = 0;

		virtual void Dispatch(uint32_t countX, uint32_t countY, uint32_t countZ) = 0;

		virtual void AddBarrier(Texture& texture, TextureLayout oldLayout, TextureLayout newLayout) = 0;

		virtual uint32_t GetBufferCount() const = 0;

		template <typename... Args> requires(sizeof...(Args) > 0)
		void BeginDebugGroup(std::string_view label, Args&&... args)
		{
#if ZE_DEBUG
			BeginDebugGroup(Util::Format(label, std::forward<Args>(args)...));
#endif
		}
		virtual void BeginDebugGroup(std::string_view label) const = 0;
		virtual void EndDebugGroup() const = 0;
	};

}