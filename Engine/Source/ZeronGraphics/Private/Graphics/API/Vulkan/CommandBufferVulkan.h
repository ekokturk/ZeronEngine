// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/CommandBuffer.h>
#	include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class BufferVulkan;
	class Texture;
	class PipelineBinding;
	class PipelineVulkan;
	class Buffer;
	class Pipeline;
	class TextureVulkan;
	class RenderPassVulkan;
	class FrameBufferVulkan;
	class FrameBuffer;
	class RenderPass;
	class GraphicsVulkan;

	/* TODO:
	 *  - Secondary Command buffers
	 *  - Multiple Command buffers
	 */

	class CommandBufferVulkan final : public CommandBuffer {
	  public:
		CommandBufferVulkan(GraphicsVulkan& graphics, uint32_t count, bool isCompute);
		~CommandBufferVulkan();

		void Begin() override;
		void BeginRenderPass(FrameBuffer* frameBuffer) override;
		void EndRenderPass() override;
		void End() override;

		// Commands
		void Clear(Color color) override;
		void SetScissor(const Vec2i& extent, const Vec2i& offset = Vec2i()) override;
		void SetViewport(const Vec2i& size, const Vec2i& position = Vec2i()) override;
		void SetPipeline(Pipeline& pipeline) override;
		void SetPipelineBinding(PipelineBinding& binding, uint32_t index = 0) override;

		void SetVertexBuffer(Buffer& vb, uint32_t slot) override;
		void SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot) override;
		void SetIndexBuffer(Buffer& ib) override;

		void CopyBuffer(Buffer& source, Buffer& destination) override;
		void UpdateBuffer(Buffer& buff, const void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule) override;
		void SetPushConstant(const void* data, uint32_t stride, ShaderType shader) override;

		void Draw(uint32_t vertexCount, uint32_t vertexStart) override;
		void DrawIndexed(uint32_t indexCount, uint32_t indexStart = 0, uint32_t vertexStart = 0) override;
		void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart, uint32_t instanceStart) override;
		void DrawInstancedIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart, uint32_t baseVertexLocation, uint32_t instanceStart) override;

		void Dispatch(uint32_t countX, uint32_t countY, uint32_t countZ) override;

		void AddBarrier(Texture& texture, TextureLayout oldLayout, TextureLayout newLayout) override;

		uint32_t GetBufferCount() const override;

		void BeginDebugGroup(std::string_view label) const override;
		void EndDebugGroup() const override;

		// Vulkan
		vk::CommandBuffer& GetCommandBufferVK();
		void ApplyImageTransitionLayoutVK(TextureVulkan& texture, TextureLayout oldLayout, TextureLayout newLayout);
		void CopyBufferToTextureVK(BufferVulkan& source, TextureVulkan& destination);

	  private:
		vk::Device& mDevice;

		vk::CommandBuffer& _getCurrent();
		const vk::CommandBuffer& _getCurrent() const;
		void _acquireNextBuffer();

		void _setImageMemoryBarrierVK(const vk::ImageMemoryBarrier& barrier, vk::PipelineStageFlagBits src, vk::PipelineStageFlagBits dst);

	  private:
		vk::UniqueCommandPool mCommandPool;
		std::vector<vk::UniqueCommandBuffer> mCommandBufferList;
		uint32_t mCurrentCommandBufferIndex;

		std::array<vk::ClearValue, 2> mClearValue;
		vk::Rect2D mScissor;

		RenderPassVulkan* mCurrentRenderPass;
		FrameBufferVulkan* mCurrentFrameBuffer;
		PipelineVulkan* mCurrentPipeline;
	};
}
#endif