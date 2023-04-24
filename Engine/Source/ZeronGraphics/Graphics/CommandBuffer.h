// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class Buffer;
	class Pipeline;
	class PipelineBinding;
	class RenderPass;
	class FrameBuffer;

	class CommandBuffer {
	  public:
		virtual ~CommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void BeginRenderPass(FrameBuffer* frameBuffer, RenderPass* renderPass = nullptr) = 0;
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
		virtual void UpdateBuffer(Buffer& buff, void* data, uint32_t sizeBytes, uint32_t offset = 0, BufferUpdateRule updateRule = BufferUpdateRule::UnmapMemory) = 0;

		virtual void Draw(uint32_t vertexCount, uint32_t vertexStart = 0) = 0;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t indexStart = 0, uint32_t vertexStart = 0) = 0;
		virtual void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart = 0, uint32_t instanceStart = 0) = 0;
		virtual void DrawInstancedIndexed(
			uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart = 0, uint32_t baseVertexLocation = 0, uint32_t instanceStart = 0
		) = 0;

		virtual void Dispatch(uint32_t countX, uint32_t countY, uint32_t countZ) = 0;

		virtual uint32_t GetBufferCount() const = 0;
	};

}