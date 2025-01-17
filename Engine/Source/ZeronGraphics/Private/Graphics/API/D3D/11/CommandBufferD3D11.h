// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D

#	include <Graphics/CommandBuffer.h>
#	include <Graphics/API/D3D/DXGI.h>

struct ID3D11DeviceContext;
struct ID3DUserDefinedAnnotation;

namespace Zeron::Gfx
{
	class PipelineBindingD3D11;
	class PipelineD3D11;
	class FrameBufferD3D11;
	class GraphicsD3D11;
	class SwapChainD3D11;

	class CommandBufferD3D11 : public CommandBuffer {
	  public:
		CommandBufferD3D11(GraphicsD3D11& graphics);
		~CommandBufferD3D11() = default;

		void Begin() override;
		void BeginRenderPass(FrameBuffer* frameBuffer) override;
		void EndRenderPass() override;
		void End() override;

		void Clear(Color color) override;
		void SetScissor(const Vec2i& extent, const Vec2i& offset) override;
		void SetViewport(const Vec2i& size, const Vec2i& position) override;

		void SetPipeline(Pipeline& pipeline) override;
		void SetPipelineBinding(PipelineBinding& binding, uint32_t index) override;

		void SetVertexBuffer(Buffer& vb, uint32_t slot) override;
		void SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot) override;
		void SetIndexBuffer(Buffer& ib) override;

		void CopyBuffer(Buffer& source, Buffer& destination) override;
		void UpdateBuffer(Buffer& buff, const void* data, uint32_t sizeBytes, uint32_t offset = 0, BufferUpdateRule updateRule = BufferUpdateRule::UnmapMemory) override;
		void SetPushConstant(const void* data, uint32_t stride, ShaderType shaderType) override;

		void Draw(uint32_t vertexCount, uint32_t vertexStart) override;
		void DrawIndexed(uint32_t indexCount, uint32_t indexStart = 0, uint32_t vertexStart = 0) override;
		void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart, uint32_t instanceStart) override;
		void DrawInstancedIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart, uint32_t baseVertexLocation, uint32_t instanceStart) override;

		void Dispatch(uint32_t countX, uint32_t countY, uint32_t countZ) override;

		void AddBarrier(Texture& texture, TextureLayout oldLayout, TextureLayout newLayout) override;

		void BeginDebugGroup(std::string_view label) const override;
		void EndDebugGroup() const override;

		uint32_t GetBufferCount() const override;

	  private:
		void _clearResources();
		void _clearPipeline();
		void _clearPipelineBinding();

		ID3D11DeviceContext* mDeviceContext;

		Color mClearColor;
		FrameBufferD3D11* mFrameBuffer;
		PipelineD3D11* mPipeline;
		PipelineBindingD3D11* mPipelineBinding;

#	if ZE_DEBUG
		Gfx::ComPtr<ID3DUserDefinedAnnotation> mDebugAnnotation;
#	endif
	};
}
#endif